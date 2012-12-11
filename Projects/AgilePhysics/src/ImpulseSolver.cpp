#include "ImpulseSolver.h"

bool ResolveCollision(PhyCollisionData* pCollisionData, float pDT, float p_cor)
{
	//If both bodies are static a physical resolve cannot be applied. Return
	if (pCollisionData->Body1->IsStatic() && pCollisionData->Body2->IsStatic())
		return true;

	//Loop through and solve for each contact set
	for (unsigned int i = 0; i < pCollisionData->Contacts.size(); i++)
	{
		//Find the normal between the contact points
		AglVector3 n = pCollisionData->Contacts[i].second - pCollisionData->Contacts[i].first;
		float d = AglVector3::length(n);
		if (d == 0.0f)
		{
			//No collision between contacts. Skip contact set.
			continue;
		}

		//Normalize the normal
		n /= d;

		//Find the vectors between the collision points and the bodies center of mass  
		AglVector3 rA = pCollisionData->Contacts[i].first - pCollisionData->Body1->GetCenterOfMass();
		AglVector3 rB = pCollisionData->Contacts[i].second - pCollisionData->Body2->GetCenterOfMass();

		//Find the total velocity of the points in their respective contact points
		AglVector3 vA = pCollisionData->Body1->GetVelocity() + AglVector3::crossProduct(pCollisionData->Body1->GetAngularVelocity(), rA);
		AglVector3 vB = pCollisionData->Body2->GetVelocity() + AglVector3::crossProduct(pCollisionData->Body2->GetAngularVelocity(), rB);

		//Find the delta velocity between the objects
		AglVector3 dV = vA - vB;

		//Find the relative velocity of the bodies in the contact points
		float dVn = AglVector3::dotProduct(dV, n);

		//Create a bias to ensure separating velocity over a certain time period

		float resolveStrength = 0.3f;	//Strength of the bias. 1.0f indicates a solution over one "frame".
		//Lower values delays the solver

		float allowedSink = ONE_METER*0.01f; //Allow bodies to sink one centimeter into each other
		float vbias = (resolveStrength / pDT) * max(d - allowedSink, 0.0f);

		//If the bodies are already moving away from each other
		if (dVn > vbias)
			continue;

		//Coefficient of Resistution, "bounciness", of the object combo.
		float COR = p_cor;

		//Calculate the impulse in the normal direction given the resistution
		float normalCounter = -COR * (dVn);

		//Ensure at least the bias is achieved
		if (normalCounter < vbias)
			normalCounter = vbias;

		//Total velocity is "Stopping Velocity" and "Bounce Velocity"
		dVn = normalCounter - dVn;

		//Add mass as a resistance factor
		float kn = pCollisionData->Body1->GetInvMass() + pCollisionData->Body2->GetInvMass();

		//Add inertia tensors as resistance factors 
		float knAng = AglVector3::dotProduct(n, 
			AglVector3::crossProduct(AglVec3Transform(AglVector3::crossProduct(rA, n), pCollisionData->Body1->GetInvInertiaWorld()), rA) + 
			AglVector3::crossProduct(AglVec3Transform(AglVector3::crossProduct(rB, n), pCollisionData->Body2->GetInvInertiaWorld()), rB));

		kn += knAng;

		//Calculate final normal impulse
		float pn = dVn / kn;

		//Apply impulses in normal direction
		pCollisionData->Body1->AddImpulse(n * pn);
		pCollisionData->Body2->AddImpulse(-n * pn);
		AglVector3 angImpulse = AglVec3TransformNormal(AglVector3::crossProduct(rA, n * pn), pCollisionData->Body1->GetInvInertiaWorld());
		AglVector3 angImpulse2 = AglVec3TransformNormal(AglVector3::crossProduct(rB, n * pn), pCollisionData->Body2->GetInvInertiaWorld());
		pCollisionData->Body1->AddAngularImpulse(angImpulse);
		pCollisionData->Body2->AddAngularImpulse(-angImpulse2);

		//TANGENT IMPULSE-----------------------------------------------------------------

		//Recompute the delta velocities. Needed to provide an appropriate new impulse in 
		//the tangent direction.
		vA = pCollisionData->Body1->GetVelocity() + 
			AglVector3::crossProduct(pCollisionData->Body1->GetAngularVelocity(), rA);
		vB = pCollisionData->Body2->GetVelocity() + 
			AglVector3::crossProduct(pCollisionData->Body2->GetAngularVelocity(), rB);

		dV = vA - vB;
		//End Recompute-------------------------------------------------------------------

		//Compute velocity along the tangent direction
		AglVector3 t = dV - n * AglVector3::dotProduct(dV, n);

		//Rolling Friction----------------------------------------------------------------
		//Check to see if rolling should be applied using the condition w*r = v
		//That is rolling friction is applied if the object is rolling without slipping.
		if (AglVector3::lengthSquared(t) <= 0.0001f)
		{
			//Body A
			AglVector3 vAt = vA - n * AglVector3::dotProduct(vA, n);

			//Is Body A rolling?
			if (AglVector3::lengthSquared(vAt) < 0.00001f && AglVector3::lengthSquared(pCollisionData->Body1->GetVelocity()) > 0 && AglVector3::lengthSquared(pCollisionData->Body1->GetAngularVelocity()) > 0)
			{
				//Calculate body A velocity in tangent direction
				vAt = pCollisionData->Body1->GetVelocity();
				vAt = vAt - n * AglVector3::dotProduct(vAt, n);

				//If A is moving in the tangent direction
				if (AglVector3::lengthSquared(vAt) > 0)
				{
					//Calculate maximum friction
					float maxFri = abs(0.05f * pn);

					//Calculate friction and clamp it to maximum if necessary
					float vAf = AglVector3::length(vAt);
					AglVector3::normalize(vAt);
					if (vAf > maxFri)
						vAf = maxFri;

					//Apply equal impulses on translational and rotational velocity
					pCollisionData->Body1->AddImpulse(-vAt * vAf * pCollisionData->Body1->GetMass());
					AglVector3 ang = -pCollisionData->Body1->GetAngularVelocity();
					AglVector3::normalize(ang);
					ang = ang * vAf / AglVector3::length(rA);
					pCollisionData->Body1->AddAngularImpulse(ang);
				}
			}

			//Body B
			AglVector3 vBt = vB - n * AglVector3::dotProduct(vB, n);

			//Is Body B rolling?
			if (AglVector3::lengthSquared(vBt) < 0.00001f && AglVector3::lengthSquared(pCollisionData->Body2->GetVelocity()) > 0 && AglVector3::lengthSquared(pCollisionData->Body2->GetAngularVelocity()) > 0)
			{
				//Calculate body B velocity in tangent direction
				vBt = pCollisionData->Body2->GetVelocity();
				vBt = vBt - n * AglVector3::dotProduct(vBt, n);

				//If B is moving in the tangent direction
				if (AglVector3::lengthSquared(vBt) > 0)
				{
					//Calculate maximum friction
					float maxFri = abs(0.05f * pn);

					//Calculate friction and clamp it to maximum if necessary
					float vBf = AglVector3::length(vBt);
					AglVector3::normalize(vBt);
					if (vBf > maxFri)
						vBf = maxFri;

					//Apply equal impulses on translational and rotational velocity
					pCollisionData->Body2->AddImpulse(-vBt * vBf * pCollisionData->Body2->GetMass());
					AglVector3 ang = -pCollisionData->Body2->GetAngularVelocity();
					AglVector3::normalize(ang);
					ang = ang * vBf / AglVector3::length(rB);
					pCollisionData->Body2->AddAngularImpulse(ang);
				}
			}
			//Tangent friction has been handled. Continue
			continue;
		}

		//Find axis of tangent movement
		AglVector3::normalize(t);

		//Add mass as a resistance factor
		float kt = pCollisionData->Body1->GetInvMass() + pCollisionData->Body2->GetInvMass();

		//Add inertia as a resistance factor
		float ktAng = AglVector3::dotProduct(t,
			AglVector3::crossProduct(AglVec3Transform(AglVector3::crossProduct(rA, t), pCollisionData->Body1->GetInvInertiaWorld()), rA) +
			AglVector3::crossProduct(AglVec3Transform(AglVector3::crossProduct(rB, t), pCollisionData->Body2->GetInvInertiaWorld()), rB));

		//Compute final resistance
		kt += ktAng;

		//Compute maximum friction
		float maxPt = abs(0.8f * pn);

		float dvT = AglVector3::dotProduct(dV, t);
		float dPt = -dvT / kt;

		//Clamp friction to max value
		if (dPt > maxPt)
			dPt = maxPt;
		else if (dPt < -maxPt)
			dPt = -maxPt;

		//Apply final tangent impulse.
		AglVector3 pt = t * dPt;
		pCollisionData->Body1->AddImpulse(pt);
		angImpulse = AglVec3TransformNormal(AglVector3::crossProduct(rA, pt), pCollisionData->Body1->GetInvInertiaWorld());
		pCollisionData->Body1->AddAngularImpulse(angImpulse);

		pCollisionData->Body2->AddImpulse(-pt);
		angImpulse2 = AglVec3TransformNormal(AglVector3::crossProduct(rB, pt), pCollisionData->Body2->GetInvInertiaWorld());

		pCollisionData->Body2->AddAngularImpulse(-angImpulse2);
	}
	return true;
}



//SAFE Before optimization
/*bool ResolveCollision(PhyCollisionData* pCollisionData, float pDT, float p_cor)
{
	//If both bodies are static a physical resolve cannot be applied. Return
	if (pCollisionData->Body1->IsStatic() && pCollisionData->Body2->IsStatic())
		return true;

	//Loop through and solve for each contact set
	for (int i = 0; i < pCollisionData->Contacts.size(); i++)
	{
		//Find the normal between the contact points
		AglVector3 n = pCollisionData->Contacts[i].second - pCollisionData->Contacts[i].first;
		float d = AglVector3::length(n);
		if (d == 0.0f)
		{
			//No collision between contacts. Skip contact set.
			continue;
		}

		//Normalize the normal
		n /= d;

		//Find the vectors between the collision points and the bodies center of mass  
		AglVector3 rA = pCollisionData->Contacts[i].first - pCollisionData->Body1->GetPosition();
		AglVector3 rB = pCollisionData->Contacts[i].second - pCollisionData->Body2->GetPosition();

		//Find the total velocity of the points in their respective contact points
		AglVector3 vA = pCollisionData->Body1->GetVelocity() + AglVector3::crossProduct(pCollisionData->Body1->GetAngularVelocity(), rA);
		AglVector3 vB = pCollisionData->Body2->GetVelocity() + AglVector3::crossProduct(pCollisionData->Body2->GetAngularVelocity(), rB);

		//Find the delta velocity between the objects
		AglVector3 dV = vA - vB;

		//Find the relative velocity of the bodies in the contact points
		float dVn = AglVector3::dotProduct(dV, n);

		//Create a bias to ensure separating velocity over a certain time period

		float resolveStrength = 0.3f;	//Strength of the bias. 1.0f indicates a solution over one "frame".
		//Lower values delays the solver

		float allowedSink = ONE_METER*0.01f; //Allow bodies to sink one centimeter into each other
		float vbias = (resolveStrength / pDT) * max(d - allowedSink, 0);

		//If the bodies are already moving away from each other
		if (dVn > vbias)
			continue;

		//Coefficient of Resistution, "bounciness", of the object combo.
		float COR = p_cor;

		//Calculate the impulse in the normal direction given the resistution
		float normalCounter = -COR * (dVn);

		//Ensure at least the bias is achieved
		if (normalCounter < vbias)
			normalCounter = vbias;

		//Total velocity is "Stopping Velocity" and "Bounce Velocity"
		dVn = normalCounter - dVn;

		//Add mass as a resistance factor
		float kn = pCollisionData->Body1->GetInvMass() + pCollisionData->Body2->GetInvMass();

		//Add inertia tensors as resistance factors 
		float knAng = AglVector3::dotProduct(n, 
			AglVector3::crossProduct(Vec3Transform(AglVector3::crossProduct(rA, n), pCollisionData->Body1->GetInvInertiaWorld()), rA) + 
			AglVector3::crossProduct(Vec3Transform(AglVector3::crossProduct(rB, n), pCollisionData->Body2->GetInvInertiaWorld()), rB));

		kn += knAng;

		//Calculate final normal impulse
		float pn = dVn / kn;

		//Apply impulses in normal direction
		pCollisionData->Body1->AddImpulse(n * pn);
		pCollisionData->Body2->AddImpulse(-n * pn);
		AglVector3 angImpulse = Vec3TransformNormal(AglVector3::crossProduct(rA, n * pn), pCollisionData->Body1->GetInvInertiaWorld());
		AglVector3 angImpulse2 = Vec3TransformNormal(AglVector3::crossProduct(rB, n * pn), pCollisionData->Body2->GetInvInertiaWorld());
		pCollisionData->Body1->AddAngularImpulse(angImpulse);
		pCollisionData->Body2->AddAngularImpulse(-angImpulse2);

		//TANGENT IMPULSE-----------------------------------------------------------------

		//Recompute the delta velocities. Needed to provide an appropriate new impulse in 
		//the tangent direction.
		vA = pCollisionData->Body1->GetVelocity() + 
			AglVector3::crossProduct(pCollisionData->Body1->GetAngularVelocity(), rA);
		vB = pCollisionData->Body2->GetVelocity() + 
			AglVector3::crossProduct(pCollisionData->Body2->GetAngularVelocity(), rB);

		dV = vA - vB;
		//End Recompute-------------------------------------------------------------------

		//Compute velocity along the tangent direction
		AglVector3 t = dV - n * AglVector3::dotProduct(dV, n);

		//Rolling Friction----------------------------------------------------------------
		//Check to see if rolling should be applied using the condition w*r = v
		//That is rolling friction is applied if the object is rolling without slipping.
		if (AglVector3::lengthSquared(t) <= 0.0001f)
		{
			//Body A
			AglVector3 vAt = vA - n * AglVector3::dotProduct(vA, n);

			//Is Body A rolling?
			if (AglVector3::lengthSquared(vAt) < 0.00001f && AglVector3::lengthSquared(pCollisionData->Body1->GetVelocity()) > 0 && AglVector3::lengthSquared(pCollisionData->Body1->GetAngularVelocity()) > 0)
			{
				//Calculate body A velocity in tangent direction
				vAt = pCollisionData->Body1->GetVelocity();
				vAt = vAt - n * AglVector3::dotProduct(vAt, n);

				//If A is moving in the tangent direction
				if (AglVector3::lengthSquared(vAt) > 0)
				{
					//Calculate maximum friction
					float maxFri = abs(0.05f * pn);

					//Calculate friction and clamp it to maximum if necessary
					float vAf = AglVector3::length(vAt);
					AglVector3::normalize(vAt);
					if (vAf > maxFri)
						vAf = maxFri;

					//Apply equal impulses on translational and rotational velocity
					pCollisionData->Body1->AddImpulse(-vAt * vAf * pCollisionData->Body1->GetMass());
					AglVector3 ang = -pCollisionData->Body1->GetAngularVelocity();
					AglVector3::normalize(ang);
					ang = ang * vAf / AglVector3::length(rA);
					pCollisionData->Body1->AddAngularImpulse(ang);
				}
			}

			//Body B
			AglVector3 vBt = vB - n * AglVector3::dotProduct(vB, n);

			//Is Body B rolling?
			if (AglVector3::lengthSquared(vBt) < 0.00001f && AglVector3::lengthSquared(pCollisionData->Body2->GetVelocity()) > 0 && AglVector3::lengthSquared(pCollisionData->Body2->GetAngularVelocity()) > 0)
			{
				//Calculate body B velocity in tangent direction
				vBt = pCollisionData->Body2->GetVelocity();
				vBt = vBt - n * AglVector3::dotProduct(vBt, n);

				//If B is moving in the tangent direction
				if (AglVector3::lengthSquared(vBt) > 0)
				{
					//Calculate maximum friction
					float maxFri = abs(0.05f * pn);

					//Calculate friction and clamp it to maximum if necessary
					float vBf = AglVector3::length(vBt);
					AglVector3::normalize(vBt);
					if (vBf > maxFri)
						vBf = maxFri;

					//Apply equal impulses on translational and rotational velocity
					pCollisionData->Body2->AddImpulse(-vBt * vBf * pCollisionData->Body2->GetMass());
					AglVector3 ang = -pCollisionData->Body2->GetAngularVelocity();
					AglVector3::normalize(ang);
					ang = ang * vBf / AglVector3::length(rB);
					pCollisionData->Body2->AddAngularImpulse(ang);
				}
			}
			//Tangent friction has been handled. Continue
			continue;
		}

		//Find axis of tangent movement
		AglVector3::normalize(t);

		//Add mass as a resistance factor
		float kt = pCollisionData->Body1->GetInvMass() + pCollisionData->Body2->GetInvMass();

		//Add inertia as a resistance factor
		float ktAng = AglVector3::dotProduct(t,
			AglVector3::crossProduct(Vec3Transform(AglVector3::crossProduct(rA, t), pCollisionData->Body1->GetInvInertiaWorld()), rA) +
			AglVector3::crossProduct(Vec3Transform(AglVector3::crossProduct(rB, t), pCollisionData->Body2->GetInvInertiaWorld()), rB));

		//Compute final resistance
		kt += ktAng;

		//Compute maximum friction
		float maxPt = abs(0.8f * pn);

		float dvT = AglVector3::dotProduct(dV, t);
		float dPt = -dvT / kt;

		//Clamp friction to max value
		if (dPt > maxPt)
			dPt = maxPt;
		else if (dPt < -maxPt)
			dPt = -maxPt;

		//Apply final tangent impulse.
		AglVector3 pt = t * dPt;
		pCollisionData->Body1->AddImpulse(pt);
		angImpulse = Vec3TransformNormal(AglVector3::crossProduct(rA, pt), pCollisionData->Body1->GetInvInertiaWorld());
		pCollisionData->Body1->AddAngularImpulse(angImpulse);

		pCollisionData->Body2->AddImpulse(-pt);
		angImpulse2 = Vec3TransformNormal(AglVector3::crossProduct(rB, pt), pCollisionData->Body2->GetInvInertiaWorld());

		pCollisionData->Body2->AddAngularImpulse(-angImpulse2);
	}
	return true;
}*/
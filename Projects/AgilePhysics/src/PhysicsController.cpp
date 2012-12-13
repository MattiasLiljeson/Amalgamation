#include "PhysicsController.h"

PhysicsController::PhysicsController(): COLLISION_REPETITIONS(5)
{
	mTimeAccum = 0;
}
PhysicsController::~PhysicsController()
{
	Clear();
}

int PhysicsController::AddSphere(AglVector3 pPosition, float pRadius, bool pUserControlled, CompoundBody* pParent)
{
	RigidBodySphere* s = new RigidBodySphere(pPosition, pRadius, pUserControlled);
	mRigidBodies.push_back(s);
	mBodies.push_back(s);
	if (pParent)
		pParent->AddChild(s);
	return mBodies.size()-1;
}
int PhysicsController::AddBox(AglVector3 pPosition, AglVector3 pSize, float pMass, AglVector3 pVelocity, AglVector3 pAngularVelocity, bool pStatic, CompoundBody* pParent)
{
	RigidBodyBox* b = new RigidBodyBox(pPosition, pSize, pMass, pVelocity, pAngularVelocity, pStatic);
	mRigidBodies.push_back(b);
	mBodies.push_back(b);
	if (pParent)
		pParent->AddChild(b);
	return mBodies.size()-1;
}
int PhysicsController::AddConvexHull(AglVector3 pPosition, float pSize, float pMass, AglVector3 pVelocity, AglVector3 pAngularVelocity, bool pStatic, CompoundBody* pParent)
{
	//Create a new convex hull shape.
	ConvexHullShape* shape = new ConvexHullShape(10);
	mConvexHullShapes.push_back(shape);

	RigidBodyConvexHull* h = new RigidBodyConvexHull(shape, pPosition, pSize, pMass, pVelocity, pAngularVelocity, pStatic);
	mRigidBodies.push_back(h);
	mBodies.push_back(h);
	if (pParent)
		pParent->AddChild(h);
	return mBodies.size()-1;
}
int PhysicsController::AddCompoundBody(AglVector3 p_position)
{
	CompoundBody* cb = new CompoundBody(p_position);
	mBodies.push_back(cb);
	mCompoundBodies.push_back(cb);
	return mBodies.size()-1;
}

void PhysicsController::DetachBodyFromCompound(CompoundBody* p_compound, RigidBody* p_body)
{
	p_compound->DetachChild(p_body);
}

void PhysicsController::Update(float pElapsedTime)
{
	mTimeAccum += pElapsedTime;
	if (mTimeAccum < 0.005f)
		return;
	pElapsedTime = mTimeAccum;
	while (mTimeAccum > 0.005f)
		mTimeAccum -= 0.005f;

	//New update method stepping
	//1) Update Velocity and Position
	//2) Perform collision detection
	//3) Revert positions and velocities
	//4) Solve collisions
	//5) Update velocities
	//6) Solve contacts
	//7) Perform shock step (Ignored for now)
	//8) Update positions
	//9) Scramble. Resort the bodies to create a "fair" collision stepping

	//1) Update Velocities and Positions
	for (unsigned int i = 0; i < mRigidBodies.size(); i++)
	{
		mRigidBodies[i]->UpdateVelocity(pElapsedTime);
		mRigidBodies[i]->UpdatePosition(pElapsedTime);
	}
	for (unsigned int i = 0; i < mCompoundBodies.size(); i++)
	{
		mCompoundBodies[i]->UpdateVelocity(pElapsedTime);
		mCompoundBodies[i]->UpdatePosition(pElapsedTime);
	}

	//2) Check for collisions
	vector<PhyCollisionData> collisions;
	for (unsigned int i = 0; i < mRigidBodies.size(); i++)
	{
		for (unsigned int j = i + 1; j < mRigidBodies.size(); j++)
		{
			if (!mRigidBodies[i]->IsStatic() || !mRigidBodies[j]->IsStatic())
			{
				if (mRigidBodies[i]->GetParent() == mRigidBodies[j]->GetParent() && 
					mRigidBodies[i]->GetParent() != NULL)
					continue;
				PhyCollisionData colData;
				if (CheckCollision(mRigidBodies[i], mRigidBodies[j], &colData))
				{
					//Solves high impulse issues for boxes
					if (colData.Contacts.size() == 4)
					{
						pair<AglVector3, AglVector3> av;
						av.first = AglVector3(0, 0, 0);
						av.second = AglVector3(0, 0, 0);
						for (int i = 0; i < 4; i++)
						{
							av.first += colData.Contacts[i].first;
							av.second += colData.Contacts[i].second;
						}
						av.first *= 0.25f;
						av.second *= 0.25f;
						pair<AglVector3, AglVector3> temp = colData.Contacts[0];
						colData.Contacts[0] = av;
						colData.Contacts.push_back(temp);		
					}
					collisions.push_back(colData);
				}
			}
		}
	}

	//3) Revert positions and velocities
	for (unsigned int i = 0; i < mRigidBodies.size(); i++)
	{
		mRigidBodies[i]->RevertVelocity();
		mRigidBodies[i]->RevertPosition();
	}
	for (unsigned int i = 0; i < mCompoundBodies.size(); i++)
	{
		mCompoundBodies[i]->RevertVelocity();
		mCompoundBodies[i]->RevertPosition();
	}
	
	//4) Solve collisions
	unsigned int rep = COLLISION_REPETITIONS;
	while (rep > 0)
	{
		for (unsigned int i = 0; i < collisions.size(); i++)
		{
			ResolveCollision(&collisions[i], pElapsedTime, 0.1f);
		}
		rep--;
	}
	
	//5) Update Velocities
	for (unsigned int i = 0; i < mRigidBodies.size(); i++)
	{
		mRigidBodies[i]->UpdateVelocity(pElapsedTime);
	}
	for (unsigned int i = 0; i < mCompoundBodies.size(); i++)
	{
		mCompoundBodies[i]->UpdateVelocity(pElapsedTime);
	}

	//6) Solve contacts
	rep = COLLISION_REPETITIONS;
	while (rep > 0)
	{
		for (unsigned int i = 0; i < collisions.size(); i++)
		{
			ResolveCollision(&collisions[i], pElapsedTime, 0.0f);
		}
		rep--;
	}


	//7) Perform shock step
	bool shockstep = false;
	if (shockstep)
	{

		//Sort the collisions
		for (unsigned int i = 0; i < collisions.size(); i++)
		{
			for (unsigned int j = 0; j < collisions.size()-1; j++)
			{
				float min1 = min(collisions[j].Body1->GetPosition().y, 
									collisions[j].Body2->GetPosition().y);
				float min2 = min(collisions[j+1].Body1->GetPosition().y, 
					collisions[j+1].Body2->GetPosition().y);
				if (min2 < min1)
				{
					PhyCollisionData t = collisions[j];
					collisions[j] = collisions[j+1];
					collisions[j+1] = t;

				}

			}
		}
		//Perform shock step - Not fully though out! Seems to fix a lot though
		//Should look into alternative resolve methods.
		//Angular velocity should not be affected. Turns out that not affecting
		//angular velocity gave weird results
		for (unsigned int i = 0; i < collisions.size(); i++)
		{
			RigidBody* b1 = collisions[i].Body1;
			float k1 = AglVector3::dotProduct(b1->GetVelocity(), b1->GetVelocity()) + AglVector3::dotProduct(b1->GetAngularVelocity(), b1->GetAngularVelocity());

			RigidBody* b2 = collisions[i].Body2;
			float k2 = AglVector3::dotProduct(b2->GetVelocity(), b2->GetVelocity()) + AglVector3::dotProduct(b2->GetAngularVelocity(), b2->GetAngularVelocity());

			if (k1 + k2 < 0.5f * ONE_METER)
			{
				ResolveCollision(&collisions[i], pElapsedTime, 0.0f);
				collisions[i].Body2->SetTempStatic(true);
				collisions[i].Body1->SetTempStatic(true);
			}
		}
	}


	//8) Update positions
	for (unsigned int i = 0; i < mRigidBodies.size(); i++)
	{
		mRigidBodies[i]->SetTempStatic(false);
		mRigidBodies[i]->UpdatePosition(pElapsedTime);
	}
	for (unsigned int i = 0; i < mCompoundBodies.size(); i++)
	{
		mCompoundBodies[i]->UpdatePosition(pElapsedTime);
	}


	//9) Scramble the bodies
	for (unsigned int i = 0; i < mRigidBodies.size(); i++)
	{
		int rnd1 = rand() % mRigidBodies.size();
		int rnd2 = rand() % mRigidBodies.size();
		RigidBody* t = mRigidBodies[rnd1];
		mRigidBodies[rnd1] = mRigidBodies[rnd2];
		mRigidBodies[rnd2] = t;
	}
}
void PhysicsController::Clear()
{
	for (unsigned int i = 0; i < mRigidBodies.size(); i++)
		delete mRigidBodies[i];
	for (unsigned int i = 0; i < mConvexHullShapes.size(); i++)
		delete mConvexHullShapes[i];

	mRigidBodies.clear();
	mConvexHullShapes.clear();
}
float PhysicsController::RaysVsObjects(vector<PhyRay> rays, RigidBody* p_ignore, AglBoundingSphere p_sphere)
{
	vector<RigidBody*> toCheck;
	for (unsigned int i = 0; i < mRigidBodies.size(); i++)
	{
		if (CheckCollision(p_sphere, mRigidBodies[i]))
		{
			toCheck.push_back(mRigidBodies[i]);
		}
	}
	float minT = 1;
	for (unsigned int i = 0; i < rays.size(); i++)
	{
		for (unsigned int j = 0; j < toCheck.size(); j++)
		{

			RayCollisionData data;
			if (toCheck[j] != p_ignore && CheckCollision(rays[i], toCheck[j], &data))
			{
				minT = min(minT, data.t);
			}
		}
	}
	return minT;
}
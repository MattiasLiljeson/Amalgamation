#include "CompoundBody.h"

void CompoundBody::ComputeInertia()
{
	//http://en.wikipedia.org/wiki/Parallel_axis_theorem
	//Uses the parallel axis theorem to compute a combined
	//Inertia tensor in world space around the center of mass
	//for the entire compound body

	//Skew-symmetric matrices are used as a supplement to
	//cross products.

	AglMatrix inertia = AglMatrix::zeroMatrix();
	AglMatrix id = AglMatrix::identityMatrix();
	for (unsigned int i = 0; i < mChildren.size(); i++)
	{
		AglMatrix orientation = mChildren[i]->GetWorld();
		orientation[15] = 1;
		orientation[14] = 0;
		orientation[13] = 0;
		orientation[12] = 0;
		AglMatrix InertiaWorld = AglMatrix::transpose(orientation) * mChildren[i]->GetLocalInertia() * orientation;
		AglVector3 r = mChildren[i]->GetLocalCenterOfMass() - GetCenterOfMass();

		//OuterProduct
		AglMatrix outer(r.x * r.x, r.x * r.y, r.x * r.z, 0, r.y * r.x, r.y * r.y, r.z * r.x, 0,
						r.z * r.x, r.z * r.y, r.z * r.x, 0, 0, 0, 0, 0);

		inertia += InertiaWorld + (id*AglVector3::dotProduct(r, r) - outer) * mChildren[i]->GetLocalMass();
	}
	mInertiaWorld = inertia;
	mInverseInertiaWorld = AglMatrix::inverse(mInertiaWorld);
}

CompoundBody::CompoundBody(AglVector3 pPosition)
{
	mLocalTransform = mPreviousLocalTransform = AglMatrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, pPosition.x, pPosition.y, pPosition.z, 1);
	mInertiaTensor = AglMatrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	mVelocity = mPreviousVelocity = mForce = mAngularVelocity = mPreviousAngularVelocity = AglVector3(0, 0, 0);
	mStatic = false;
	mMass = 0;
	mInvMass = FLT_MAX;
	mTempStatic = false;
}
CompoundBody::~CompoundBody()
{

}
void CompoundBody::AddChild(RigidBody* pRigidBody)
{
	if (pRigidBody->IsStatic() != mStatic)
	{
		//CANNOT COMBINE STATIC AND NON-STATIC COMPONENTS
		float k = 0;
		k = 1.0f / k;
		return;
	}
	mChildren.push_back(pRigidBody);
	pRigidBody->SetParent(this);
}
void CompoundBody::DetachChild(RigidBody* pRigidBody)
{
	for (unsigned int i = 0; i < mChildren.size(); i++)
	{
		if (mChildren[i] == pRigidBody)
		{
			mChildren[i] = mChildren.back();
			mChildren.pop_back();
			pRigidBody->SetParent(NULL);
			break;
		}
	}
}
float CompoundBody::GetMass()
{
	float m = 0;
	for (unsigned int i = 0; i < mChildren.size(); i++)
		m += mChildren[i]->GetLocalMass();
	return m;
}
float CompoundBody::GetInvMass()
{
	return 1.0f / GetMass();
}
AglMatrix CompoundBody::GetWorld()
{
	return mLocalTransform;
}
AglVector3 CompoundBody::GetVelocity()
{
	return mVelocity;
}
AglVector3 CompoundBody::GetAngularVelocity()
{
	return mAngularVelocity;
}
AglVector3 CompoundBody::GetCenterOfMass()
{
	AglVector3 com(0, 0, 0);
	float m = 0;
	for (unsigned int i = 0; i < mChildren.size(); i++)
	{
		com += mChildren[i]->GetLocalCenterOfMass() * mChildren[i]->GetLocalMass();
		m += mChildren[i]->GetLocalMass();
	}
	com /= m;
	return com;
}

void CompoundBody::AddImpulse(AglVector3 pImpulse)
{
	if (!mStatic && !mTempStatic)
		mVelocity += pImpulse * GetInvMass();
}
void CompoundBody::AddAngularImpulse(AglVector3 pAngularImpulse)
{
	if (!mStatic && !mTempStatic)
		mAngularVelocity += pAngularImpulse;
}
void CompoundBody::UpdateVelocity(float pElapsedTime)
{
	//Updates velocity and angular velocity only
	if (!mStatic && !mTempStatic)
	{
		//mForce += AglVector3(0, -9.815f, 0);
		mPreviousVelocity = mVelocity;
		mPreviousAngularVelocity = mAngularVelocity;
		mVelocity += mForce * pElapsedTime;
		mForce = AglVector3(0, 0, 0);

		//Damping
		mVelocity *= (1 - 0.9f * pElapsedTime);
		mAngularVelocity *= (1 - 0.9f * pElapsedTime);
	}
}
void CompoundBody::UpdatePosition(float pElapsedTime)
{
	//Updates mWorld only
	if (!mStatic && !mTempStatic)
	{
		mPreviousLocalTransform = mLocalTransform;
		mLocalTransform[12] += mVelocity.x * pElapsedTime;
		mLocalTransform[13] += mVelocity.y * pElapsedTime;
		mLocalTransform[14] += mVelocity.z * pElapsedTime;

		float a = AglVector3::length(mAngularVelocity);
		if (a > 0)
		{
			AglVector3 s, t;
			AglQuaternion r;
			AglMatrix::matrixToComponents(mLocalTransform, s, r, t);
			AglQuaternion r2 = AglQuaternion::constructFromAngularVelocity(mAngularVelocity * pElapsedTime);
			r = r2 * r;
			AglMatrix::componentsToMatrix(mLocalTransform, AglVector3(1, 1, 1), r, t);
		}
		ComputeInertia();
	}
}
void CompoundBody::RevertVelocity()
{
	if (!mStatic && !mTempStatic)
	{
		mVelocity = mPreviousVelocity;
		mAngularVelocity = mPreviousAngularVelocity;
	}
}

void CompoundBody::RevertPosition()
{
	if (!mStatic && !mTempStatic)
		mLocalTransform = mPreviousLocalTransform;
}

AglMatrix CompoundBody::GetInverseInertiaWorld()
{
	return mInverseInertiaWorld;
}
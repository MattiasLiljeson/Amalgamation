#include "RigidBody.h"
#include <cmath>
#include <iostream>
#include "CompoundBody.h"

void RigidBody::ComputeInertia()
{
    mInvInertiaWorld = AglMatrix(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

    AglMatrix orientation = mLocalTransform;
	orientation[15] = 1;
	orientation[14] = 0;
	orientation[13] = 0;
	orientation[12] = 0;

	AglMatrix m = AglMatrix::inverse(mInertiaTensor);

	//Used inverse before. Should be the same.
	mInvInertiaWorld = AglMatrix::transpose(orientation) * m * orientation;
}

void RigidBody::SetInertiaTensor(AglMatrix pInertiaTensor)
{
	mInertiaTensor = pInertiaTensor;
}
RigidBody::RigidBody()
{
	mLocalTransform = mPreviousLocalTransform = AglMatrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	mInertiaTensor = AglMatrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	mVelocity = mPreviousVelocity = mPreviousAngularVelocity = mForce = mAngularVelocity = AglVector3(0, 0, 0);
    mStatic = false;
    mMass = ONE_KILOGRAM;
	mInvMass = 1.0f / mMass;
	mUserControlled = false;
	mTempStatic = false;
	mParent = NULL;
	ComputeInertia();
}
RigidBody::RigidBody(AglVector3 pPosition)
{
	mLocalTransform = mPreviousLocalTransform = AglMatrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, pPosition.x, pPosition.y, pPosition.z, 1);
	mInertiaTensor = AglMatrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	mVelocity = mPreviousVelocity = mForce = mAngularVelocity = mPreviousAngularVelocity = AglVector3(0, 0, 0);
    mStatic = false;
    mMass = ONE_KILOGRAM;
	mInvMass = 1.0f / mMass;
	mUserControlled = false;
	mTempStatic = false;
	mParent = NULL;
	ComputeInertia();
}
RigidBody::RigidBody(AglMatrix pCoordinateSystem, AglVector3 pPosition, float pMass, AglVector3 pVelocity, AglVector3 pAngularVelocity, bool pStatic, bool pUserControlled)
{
	AglMatrix mat = pCoordinateSystem * AglMatrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, pPosition.x, pPosition.y, pPosition.z, 1);
	mLocalTransform = mPreviousLocalTransform = mat;
	mInertiaTensor = AglMatrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	mVelocity = mPreviousVelocity = pVelocity;
	mAngularVelocity = mPreviousAngularVelocity = pAngularVelocity;
	mForce = AglVector3(0, 0, 0);
    mStatic = pStatic;
    mMass = pMass;
	mInvMass = 1.0f / mMass;
	mUserControlled = pUserControlled;
	mTempStatic = false;
	mParent = NULL;

	/*if (pPosition.z == -40)
	{
		AglQuaternion quat = AglQuaternion::constructFromAxisAndAngle(AglVector3(0, 1, 0), 1.0f);
		AglMatrix::componentsToMatrix(mLocalTransform, AglVector3(1, 1, 1), quat, pPosition);
	}*/

	ComputeInertia();
}
RigidBody::RigidBody(AglVector3 pPosition, float pMass, AglVector3 pVelocity, AglVector3 pAngularVelocity, bool pStatic, bool pUserControlled)
{
	mLocalTransform = mPreviousLocalTransform = AglMatrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, pPosition.x, pPosition.y, pPosition.z, 1);
	mInertiaTensor = AglMatrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	mVelocity = mPreviousVelocity = pVelocity;
	mAngularVelocity = mPreviousAngularVelocity = pAngularVelocity;
	mForce = AglVector3(0, 0, 0);
    mStatic = pStatic;
    mMass = pMass;
	mInvMass = 1.0f / mMass;
	mUserControlled = pUserControlled;
	mTempStatic = false;
	mParent = NULL;

	/*if (pPosition.z == -40)
	{
		AglQuaternion quat = AglQuaternion::constructFromAxisAndAngle(AglVector3(0, 1, 0), 1.0f);
		AglMatrix::componentsToMatrix(mLocalTransform, AglVector3(1, 1, 1), quat, pPosition);
	}*/

	ComputeInertia();
}
RigidBody::~RigidBody()
{

}
float RigidBody::GetMass() const
{
	if (mParent)
		return mParent->GetMass();
	else
		return GetLocalMass();
}
float RigidBody::GetLocalMass() const
{
	return mMass;
}
float RigidBody::GetInvMass() const
{
	if (mParent)
		return mParent->GetInvMass();
	else
		return GetInvLocalMass();
}
float RigidBody::GetInvLocalMass() const
{
	if (mStatic || mTempStatic)
		return 0;
	return mInvMass;
}
AglVector3	RigidBody::GetPosition() const
{
	if (mParent)
	{
		AglMatrix parent = mParent->GetWorld();
		AglMatrix w = mLocalTransform;
		w *= parent;
		return AglVector3(w[12], w[13], w[14]);
	}
	else
		return GetLocalPosition();
}
AglVector3 RigidBody::GetLocalPosition() const
{
	return AglVector3(mLocalTransform[12], mLocalTransform[13], mLocalTransform[14]);
}
AglMatrix RigidBody::GetWorld() const
{
	if (mParent)
	{
		AglMatrix parent = mParent->GetWorld();
		AglMatrix w = mLocalTransform;
		w *= parent;
		return w;
	}
	else
	{
		return mLocalTransform;
	}
}
AglMatrix RigidBody::GetLocalTransform() const
{
	return mLocalTransform;
}
vector<AglVector3> RigidBody::GetAxesWorld() const
{
	AglMatrix w = GetWorld();
	vector<AglVector3> axes(3);
	axes[0] = AglVector3(w[0], w[1], w[2]);
	axes[1] = AglVector3(w[4], w[5], w[6]);
	axes[2] = AglVector3(w[8], w[9], w[10]);

	return axes;
}
AglVector3 RigidBody::GetVelocity() const
{
	if (mStatic || mTempStatic)
		return AglVector3(0, 0, 0);

	if (mParent)
		return mParent->GetVelocity() + mVelocity;
	else
		return mVelocity;
}
AglVector3 RigidBody::GetAngularVelocity() const
{
	if (mStatic || mTempStatic)
		return AglVector3(0, 0, 0);

	if (mParent)
		return mParent->GetAngularVelocity() + mAngularVelocity;
	else
		return mAngularVelocity;
}
AglMatrix RigidBody::GetLocalInertia() const
{
	return mInertiaTensor;
}
AglMatrix RigidBody::GetInvInertiaWorld() const
{
	//Parent should not always return zero matrix. Temporary solution.
    if (mStatic || mTempStatic || this->mUserControlled)
        return AglMatrix(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

	if (mParent)
		return mParent->GetInverseInertiaWorld();

	return mInvInertiaWorld;
}
bool RigidBody::IsStatic() const
{
	return mStatic || mTempStatic;
}

void RigidBody::AddImpulse(AglVector3 pImpulse)
{
	if (_isnan((double)pImpulse.x) || _isnan((double)pImpulse.y) || _isnan((double)pImpulse.z))
	{
		mLocalTransform.data[0] = 0;
		return;
	}

	if (mParent)
		mParent->AddImpulse(pImpulse);
	else
	{
		if (!mStatic && !mTempStatic)
			mVelocity += pImpulse * GetInvMass();
	}
}
void RigidBody::AddAngularImpulse(AglVector3 pAngularImpulse)
{
	if (_isnan((double)pAngularImpulse.x) || _isnan((double)pAngularImpulse.y) || _isnan((double)pAngularImpulse.z))
	{
		mLocalTransform.data[0] = 0;
		return;
	}
	if (mParent)
		mParent->AddAngularImpulse(pAngularImpulse);
	else
	{
		if (!mStatic && !mTempStatic)
			mAngularVelocity += pAngularImpulse;
	}
}

AglBoundingSphere RigidBody::GetBoundingSphere() const
{
	AglBoundingSphere s;
	s.radius = mBoundingSphere.radius;
	s.position = this->GetPosition();
	return s;
}

void RigidBody::RevertVelocity()
{
	if (!mStatic && !mTempStatic)
	{
		mVelocity = mPreviousVelocity;
		mAngularVelocity = mPreviousAngularVelocity;
	}
}

void RigidBody::RevertPosition()
{
	if (!mStatic && !mTempStatic)
		mLocalTransform = mPreviousLocalTransform;
}
AglVector3 RigidBody::GetCenterOfMass()
{
	if (mParent)
		return mParent->GetCenterOfMass();
	else
		return GetLocalCenterOfMass();
}
void RigidBody::SetParent(CompoundBody* pParent)
{
	if (mParent && !pParent)
	{
		mLocalTransform = this->GetWorld();
		mVelocity = this->GetVelocity();
		mAngularVelocity = this->GetAngularVelocity();

	}
	mParent = pParent;
}
CompoundBody* RigidBody::GetParent()
{
	return mParent;
}
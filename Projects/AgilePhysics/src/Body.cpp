#include "Body.h"

Body::Body()
{
	mActive = true;
}
Body::~Body()
{

}

void Body::UpdateVelocity(float pElapsedTime)
{
	if (!mActive)
		return;

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
void Body::UpdatePosition(float pElapsedTime)
{
	if (!mActive)
		return;

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
		    
			AglVector3 right = mLocalTransform.GetRight();
			AglVector3 up = mLocalTransform.GetUp();
			AglVector3 forward = mLocalTransform.GetForward();

			r2.transformVector(right);
			r2.transformVector(up);
			r2.transformVector(forward);

			mLocalTransform.SetRight(right);
			mLocalTransform.SetUp(up);
			mLocalTransform.SetForward(forward);

			//r = r2 * r;
			//AglMatrix::componentsToMatrix(mLocalTransform, AglVector3(1, 1, 1), r, t);
		}
		ComputeInertia();
	}
}

bool Body::IsCompoundBody()
{
	return false;
}

void Body::Activate()
{
	mActive = true;
}
void Body::Inactivate()
{
	mActive = false;
}
bool Body::IsActive()
{
	return mActive;
}
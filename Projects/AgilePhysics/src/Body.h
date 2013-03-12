// =======================================================================================
//                                      Body
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # Body
/// Detailed description.....
/// Created on: 5-12-2012 
///---------------------------------------------------------------------------------------
#ifndef BODY_H
#define BODY_H

#include "PhyUtility.h"
#include "AglBoundingSphere.h"

class Body
{
protected:
	AglMatrix		mLocalTransform;
	AglMatrix		mPreviousLocalTransform;
	AglMatrix		mInertiaTensor;
	AglMatrix		mInvInertiaWorld;
	AglVector3		mVelocity;
	AglVector3		mPreviousVelocity;
	AglVector3		mForce;
	AglVector3		mAngularVelocity;
	AglVector3		mPreviousAngularVelocity;
	bool		mStatic;
	bool		mTempStatic;
	float		mMass;
	float		mInvMass;

	bool mActive;
public:
	Body();
	virtual ~Body();
	virtual AglMatrix GetWorld() const = 0;
	virtual AglVector3 GetVelocity() const = 0;
	virtual AglVector3 GetAngularVelocity() const = 0;
	virtual void AddImpulse(AglVector3 pImpulse) = 0;
	virtual void AddAngularImpulse(AglVector3 pAngularImpulse, bool p_propagate = true) = 0;
	virtual void UpdateVelocity(float pElapsedTime);
	virtual void UpdatePosition(float pElapsedTime);
	virtual void RevertVelocity() = 0;
	virtual void RevertPosition() = 0;
	bool IsStatic() const; //NOT

	virtual bool IsCompoundBody();

	void Activate();
	void Inactivate();
	bool IsActive();

	virtual void ComputeInertia() = 0;

	void setTransform(AglMatrix p_setTransform);

	virtual float GetMass() const = 0;
};

#endif // BODY_H
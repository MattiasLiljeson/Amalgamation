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
public:
	Body();
	virtual ~Body();
	virtual AglMatrix GetWorld() const = 0;
};

#endif // BODY_H
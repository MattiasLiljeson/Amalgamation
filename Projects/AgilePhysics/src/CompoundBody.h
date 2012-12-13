// =======================================================================================
//                                      CompoundBody
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # Header
/// Detailed description.....
/// Created on: 5-12-2012 
///---------------------------------------------------------------------------------------
#ifndef HEADER_H
#define HEADER_H

#include "RigidBody.h"

class CompoundBody : public Body
{
private:
	vector<RigidBody*> mChildren;

	AglMatrix mInertiaWorld;
	AglMatrix mInverseInertiaWorld;

private:
	void ComputeInertia();
public:
	CompoundBody(AglVector3 pPosition);
	virtual ~CompoundBody();

	void AddChild(RigidBody* pRigidBody);
	void DetachChild(RigidBody* pRigidBody);
	float GetMass();
	float GetInvMass();
	AglMatrix GetWorld() const;
	AglVector3 GetVelocity();
	AglVector3 GetAngularVelocity();
	AglVector3 GetCenterOfMass();

	void AddImpulse(AglVector3 pImpulse);
	void AddAngularImpulse(AglVector3 pAngularImpulse);

	void UpdateVelocity(float pElapsedTime);
	void UpdatePosition(float pElapsedTime);
	void RevertVelocity();
	void RevertPosition();

	AglMatrix GetInverseInertiaWorld();
};

#endif // HEADER_H
#ifndef RIGIDBODYBOX_H
#define RIGIDBODYBOX_H

#include "RigidBody.h"
#include "AglOBB.h"

struct BoxInitData
{
	AglMatrix	World;
	AglVector3	Size;
	float		Mass;
	AglVector3	Velocity;
	AglVector3	AngularVelocity;
	bool		Static;
	bool		ImpulseEnabled;
};

class RigidBodyBox: public RigidBody
{
private:
	AglVector3 mSize;

	//Cached Box corners - Must be updated each time position is updated.
	vector<AglVector3> mCornersWorld;
private:
	void CalculateInertiaTensor();
public:
	RigidBodyBox(AglVector3 pPosition, AglVector3 pSize, float pMass, AglVector3 pVelocity, AglVector3 pAngularVelocity, bool pStatic, bool pImpulseEnabled);
	RigidBodyBox(AglOBB pShape, float pMass, AglVector3 pVelocity, AglVector3 pAngularVelocity, bool pStatic, bool pImpulseEnabled);
	RigidBodyBox(BoxInitData pInitData); 
	~RigidBodyBox();
	RigidBodyType GetType();
	const vector<AglVector3>& GetCorners();
	AglVector3 GetSizeAsVector3();
	AglMatrix	GetSizeAsMatrix();
	void UpdatePosition(float pElapsedTime);
	AglVector3 GetLocalCenterOfMass(){ return GetPosition(); }
};

#endif
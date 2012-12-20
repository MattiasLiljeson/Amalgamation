#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "Body.h"

//COMPOUND OBJECTS DO NOT WORK WITH STATIC OBJECTS!!!!

enum RigidBodyType
{
	SPHERE, BOX, CONVEXHULL, MESH
};

class CompoundBody;

class RigidBody : public Body
{
private:
		CompoundBody* mParent;

protected:
		AglBoundingSphere mBoundingSphere;
		bool mUserControlled;
protected:
	virtual void CalculateInertiaTensor() = 0;
	void SetInertiaTensor(AglMatrix pTensor);
	void calcInvInertia();
public:
	RigidBody();
	RigidBody(AglVector3 pPosition);
	RigidBody(AglVector3 pPosition, float pMass, AglVector3 pVelocity, AglVector3 pAngularVelocity, bool pStatic = false, bool pUserControlled = false);
	RigidBody(AglMatrix pCoordinateSystem, AglVector3 pPosition, float pMass, AglVector3 pVelocity, AglVector3 pAngularVelocity, bool pStatic = false, bool pUserControlled = false);
	virtual ~RigidBody();
	virtual RigidBodyType	GetType() = 0;

	//One local and one for compound
	float					GetMass() const;
	float					GetLocalMass() const;

	//One local and one for compound
	float					GetInvMass() const;
	float					GetInvLocalMass() const;

	//One local and one for compound
	AglVector3					GetPosition() const;
	AglVector3					GetLocalPosition() const;

	//One local and one for compound
	AglMatrix					GetWorld() const;
	AglMatrix					GetLocalTransform() const;

	//Only one copy. Local axes cannot be received right now
	vector<AglVector3>			GetAxesWorld() const;
	
	//Only one copy. Local velocity cannot be received right now
	AglVector3					GetVelocity() const;

	//Only one copy. Local angular velocity cannot be received right now
	AglVector3					GetAngularVelocity() const;

	AglMatrix					GetLocalInertia() const;
	AglMatrix					GetInvInertiaWorld() const; //Wait with this for now. Simply return Matrix zero
	bool					IsStatic() const; //NOT
	void					SetTempStatic(bool pValue){ mTempStatic = pValue;} //NOT

	void AddImpulse(AglVector3 pImpulse);
	void AddAngularImpulse(AglVector3 pAngularImpulse);

	virtual void UpdateVelocity(float pElapsedTime); //NOT
	virtual void UpdatePosition(float pElapsedTime); //NOT
	void RevertVelocity(); //NOT
	void RevertPosition(); //NOT
	virtual AglBoundingSphere GetBoundingSphere() const; //NOT

	AglVector3 GetCenterOfMass();
	virtual AglVector3 GetLocalCenterOfMass() = 0;

	void SetParent(CompoundBody* pParent);
	CompoundBody* GetParent();
};

#endif
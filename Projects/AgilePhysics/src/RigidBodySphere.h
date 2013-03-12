#ifndef RIGIDBODYSPHERE_H
#define RIGIDBODYSPHERE_H

#include "RigidBody.h"

//Actually a specially designed sphere used to 
//handle the spaceship

class RigidBodySphere: public RigidBody
{
private:
	float mRadius;
private:
	void CalculateInertiaTensor();
public:
	RigidBodySphere(AglVector3 pPosition, float pRadius, bool userControlled = false, bool pImpulseEnabled = true);
	RigidBodySphere(AglMatrix p_world, float p_radius, float p_mass, AglVector3 p_velocity, AglVector3 p_angularVelocity, bool p_static = false, CompoundBody* pParent = NULL, 
		bool pImpulseEnabled = NULL);
	~RigidBodySphere();
	RigidBodyType GetType();
	float			GetRadius() const;
	AglVector3 GetLocalCenterOfMass(){ return GetPosition(); }
};

#endif
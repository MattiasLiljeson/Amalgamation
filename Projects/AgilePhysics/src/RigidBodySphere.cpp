#include "RigidBodySphere.h"

void RigidBodySphere::CalculateInertiaTensor()
{
	float inertia = 0.4f * GetMass() * mRadius * mRadius;
    SetInertiaTensor(AglMatrix(inertia, 0, 0, 0, 0, inertia, 0, 0, 0, 0, inertia, 0, 0, 0, 0, 1));

	//Above is the correct tensor. Below is the one used to avoid angular momentum
	if (mUserControlled)
		SetInertiaTensor(AglMatrix(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
}
RigidBodySphere::RigidBodySphere(AglVector3 pPosition, float pRadius, bool userControlled, bool pImpulseEnabled): RigidBody(pPosition, ONE_KILOGRAM, AglVector3(0, 0, 0), AglVector3(0, 0, 0), false, false, pImpulseEnabled)
{
	mRadius = pRadius;
	mBoundingSphere.radius = pRadius;
	CalculateInertiaTensor();
}
RigidBodySphere::RigidBodySphere(AglMatrix p_world, float p_radius, float p_mass, AglVector3 p_velocity, AglVector3 p_angularVelocity, bool p_static, 
				CompoundBody* pParent, bool pImpulseEnabled) :
				 RigidBody(p_world, p_mass, p_velocity, p_angularVelocity, p_static, false, pImpulseEnabled)
{
	
	mRadius = p_radius;
	mBoundingSphere.radius = p_radius;
	CalculateInertiaTensor();
	SetParent(pParent);
}
RigidBodySphere::~RigidBodySphere()
{

}
RigidBodyType RigidBodySphere::GetType()
{
	return SPHERE;
}
float RigidBodySphere::GetRadius() const
{
	return mRadius;
}
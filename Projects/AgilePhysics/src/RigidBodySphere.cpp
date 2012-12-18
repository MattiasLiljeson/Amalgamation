#include "RigidBodySphere.h"

void RigidBodySphere::CalculateInertiaTensor()
{
	float inertia = 0.4f * GetMass() * mRadius * mRadius;
    SetInertiaTensor(AglMatrix(inertia, 0, 0, 0, 0, inertia, 0, 0, 0, 0, inertia, 0, 0, 0, 0, 1));

	//Above is the correct tensor. Below is the one used to avoid angular momentum
	if (mUserControlled)
		SetInertiaTensor(AglMatrix(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
}
RigidBodySphere::RigidBodySphere(AglVector3 pPosition, float pRadius, bool userControlled): RigidBody(pPosition, ONE_KILOGRAM, AglVector3(0, 0, 0), AglVector3(0, 0, 0), false, true)
{
	mRadius = pRadius;
	mBoundingSphere.radius = pRadius;
	CalculateInertiaTensor();
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
#include "RigidBodyBox.h"

void RigidBodyBox::CalculateInertiaTensor()
{
	float xfactor = (4 * mSize[0] * mSize[0]);
    float yfactor = (4 * mSize[1] * mSize[1]);
    float zfactor = (4 * mSize[2] * mSize[2]);
    float x = (yfactor + zfactor) * GetMass() / 12.0f;
    float y = (xfactor + zfactor) * GetMass() / 12.0f;
    float z = (xfactor + yfactor) * GetMass() / 12.0f;
	SetInertiaTensor(AglMatrix(x, 0, 0, 0, 0, y, 0, 0, 0, 0, z, 0, 0, 0, 0, 1));
}
RigidBodyBox::RigidBodyBox(AglVector3 pPosition, AglVector3 pSize, float pMass, AglVector3 pVelocity, AglVector3 pAngularVelocity, bool pStatic, bool pImpulseEnabled): RigidBody(pPosition, pMass, pVelocity, pAngularVelocity, pStatic, false, pImpulseEnabled)
{
	mSize = pSize;
	CalculateInertiaTensor();

	//Initialize bounding sphere
	mBoundingSphere.position = pPosition;
	mBoundingSphere.radius = AglVector3::length(mSize)*0.5f;
}
RigidBodyBox::RigidBodyBox(AglOBB pShape, float pMass, AglVector3 pVelocity, AglVector3 pAngularVelocity, bool pStatic, bool pImpulseEnabled)
	: RigidBody(pShape.world, pMass, pVelocity, pAngularVelocity, pStatic, false, pImpulseEnabled)
{
	mSize = pShape.size;
	CalculateInertiaTensor();

	//Initialize bounding sphere
	mBoundingSphere.position = pShape.world.GetTranslation();
	mBoundingSphere.radius = AglVector3::length(mSize)*0.5f;
}
RigidBodyBox::RigidBodyBox(BoxInitData pInitData)
	: RigidBody(pInitData.World, pInitData.Mass, pInitData.Velocity, pInitData.AngularVelocity, pInitData.Static, pInitData.ImpulseEnabled)
{
	mSize = pInitData.Size;
	CalculateInertiaTensor();

	//Initialize bounding sphere
	mBoundingSphere.position = pInitData.World.GetTranslation();
	mBoundingSphere.radius = AglVector3::length(mSize)*0.5f;
}

RigidBodyBox::~RigidBodyBox()
{

}
RigidBodyType RigidBodyBox::GetType()
{
	return BOX;
}
const vector<AglVector3>& RigidBodyBox::GetCorners()
{
	if (mCornersWorld.size() == 0)
	{
		AglVector3 HalfSize = mSize / 2;
		mCornersWorld.push_back(AglVector3(-HalfSize[0], -HalfSize[1], -HalfSize[2]));
		mCornersWorld.push_back(AglVector3( HalfSize[0], -HalfSize[1], -HalfSize[2]));
		mCornersWorld.push_back(AglVector3(-HalfSize[0],  HalfSize[1], -HalfSize[2]));
		mCornersWorld.push_back(AglVector3(-HalfSize[0], -HalfSize[1],  HalfSize[2]));
		mCornersWorld.push_back(AglVector3( HalfSize[0],  HalfSize[1], -HalfSize[2]));
		mCornersWorld.push_back(AglVector3( HalfSize[0], -HalfSize[1],  HalfSize[2]));
		mCornersWorld.push_back(AglVector3(-HalfSize[0],  HalfSize[1],  HalfSize[2]));
		mCornersWorld.push_back(AglVector3( HalfSize[0],  HalfSize[1],  HalfSize[2]));
		for (int i = 0; i < 8; i++)
		{
			AglVec3Transform(mCornersWorld[i], GetWorld());
		}
	}
	return mCornersWorld;
}
AglVector3 RigidBodyBox::GetSizeAsVector3()
{
	return mSize;
}
AglMatrix	RigidBodyBox::GetSizeAsMatrix()
{
	return AglMatrix(mSize[0], 0, 0, 0, 0, mSize[1], 0, 0, 0, 0, mSize[2], 0, 0, 0, 0, 1);
}
void RigidBodyBox::UpdatePosition(float pElapsedTime)
{
	RigidBody::UpdatePosition(pElapsedTime);
	mCornersWorld.clear();
	AglVector3 HalfSize = mSize / 2;
	mCornersWorld.push_back(AglVector3(-HalfSize[0], -HalfSize[1], -HalfSize[2]));
	mCornersWorld.push_back(AglVector3( HalfSize[0], -HalfSize[1], -HalfSize[2]));
	mCornersWorld.push_back(AglVector3(-HalfSize[0],  HalfSize[1], -HalfSize[2]));
	mCornersWorld.push_back(AglVector3(-HalfSize[0], -HalfSize[1],  HalfSize[2]));
	mCornersWorld.push_back(AglVector3( HalfSize[0],  HalfSize[1], -HalfSize[2]));
	mCornersWorld.push_back(AglVector3( HalfSize[0], -HalfSize[1],  HalfSize[2]));
	mCornersWorld.push_back(AglVector3(-HalfSize[0],  HalfSize[1],  HalfSize[2]));
	mCornersWorld.push_back(AglVector3( HalfSize[0],  HalfSize[1],  HalfSize[2]));
	for (int i = 0; i < 8; i++)
	{
		AglVec3Transform(mCornersWorld[i], GetWorld());
	}
}
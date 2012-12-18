#include "RigidBodyConvexHull.h"

void RigidBodyConvexHull::CalculateInertiaTensor()
{
	//Could be done using an exact algorithm or an approximation
	//An approximation using a box or a sphere would probably be sufficient

	//Current solution is an approximation using an AABB
	vector<AglVector3> points = mShape->GetVertices();
	AglVector3 minP, maxP;
	minP = maxP = points[0];
	for (unsigned int i = 1; i < points.size(); i++)
	{
		minP = AglVector3(min(minP[0], points[i][0]), min(minP[1], points[i][1]), min(minP[2], points[i][2]));
		maxP = AglVector3(max(maxP[0], points[i][0]), max(maxP[1], points[i][1]), max(maxP[2], points[i][2]));
	}
	AglVector3 size = maxP - minP;
	size = size * mSize;

	float xfactor = (4 * size[0] * size[0]);
    float yfactor = (4 * size[1] * size[1]);
    float zfactor = (4 * size[2] * size[2]);
    float x = (yfactor + zfactor) * GetMass() / 12.0f;
    float y = (xfactor + zfactor) * GetMass() / 12.0f;
    float z = (xfactor + yfactor) * GetMass() / 12.0f;
	SetInertiaTensor(AglMatrix(x, 0, 0, 0, 0, y, 0, 0, 0, 0, z, 0, 0, 0, 0, 1));
}
RigidBodyConvexHull::RigidBodyConvexHull(ConvexHullShape* pShape, AglVector3 pPosition, float pSize, float pMass, AglVector3 pVelocity, AglVector3 pAngularVelocity, bool pStatic): RigidBody(pPosition, pMass, pVelocity, pAngularVelocity, pStatic)
{
	mShape = pShape;
	mSize  = pSize;

	//pSize is actually the radius. Not too clear!
	mBoundingSphere.position = pPosition;
	mBoundingSphere.radius = pSize;

	CalculateInertiaTensor();
}
RigidBodyConvexHull::~RigidBodyConvexHull()
{

}
RigidBodyType RigidBodyConvexHull::GetType()
{
	return CONVEXHULL;
}
const vector<AglVector3>& RigidBodyConvexHull::GetExteriorPoints()
{
	if (mPointsWorld.size() == 0)
	{
		mPointsWorld = mShape->GetVertices();
		AglMatrix w = GetWorld();
		for (unsigned int i = 0; i < mPointsWorld.size(); i++)
		{
			mPointsWorld[i] *= mSize;
			AglVec3Transform(mPointsWorld[i], GetWorld());
		}
	}
	return mPointsWorld;
}
void RigidBodyConvexHull::UpdatePosition(float pElapsedTime)
{
	RigidBody::UpdatePosition(pElapsedTime);
	mPointsWorld = mShape->GetVertices();
	AglMatrix w = GetWorld();
	for (unsigned int i = 0; i < mPointsWorld.size(); i++)
	{
		mPointsWorld[i] *= mSize;
		AglVec3Transform(mPointsWorld[i], GetWorld());
	}
}
vector<PhyPlane> RigidBodyConvexHull::GetPlanes()
{
	if (mPointsWorld.size() == 0)
	{
		mPointsWorld = mShape->GetVertices();
		AglMatrix w = GetWorld();
		for (unsigned int i = 0; i < mPointsWorld.size(); i++)
		{
			mPointsWorld[i] *= mSize;
			AglVec3Transform(mPointsWorld[i], GetWorld());
		}
	}
	vector<AglVector3> verts = mPointsWorld;
	vector<unsigned int> ind = mShape->GetIndices();

	vector<PhyPlane> planes;
	for (unsigned int i = 0; i < ind.size(); i+=3)
	{
		AglVector3 n = AglVector3::crossProduct(verts[ind[i+1]]-verts[ind[i]], verts[ind[i+2]]-verts[ind[i]]);
		AglVector3::normalize(n);
		float d = AglVector3::dotProduct(n, verts[ind[i+2]]);
		planes.push_back(PhyPlane(n, d));
	}

	return planes;
}
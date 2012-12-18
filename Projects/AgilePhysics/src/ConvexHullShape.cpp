#include "ConvexHullShape.h"

ConvexHullShape::ConvexHullShape(int pClusterSize)
{
	//Create some random points
	vector<AglVector3> points = vector<AglVector3>();
	for (int i = 0; i < pClusterSize; i++)
	{
		float x = AglRandomFloat(-1, 1);
		float y = AglRandomFloat(-1, 1);
		float z = AglRandomFloat(-1, 1);
		AglVector3 p = AglVector3(x, y, z);
		points.push_back(p);
	}

	mHull = new AglHull(points);

	//Move the shape so that the center lies at the origin
	//Normalize it such that the furthest point lies at 
	//a distance of 1.0f from the origin.
	AglBoundingSphere bs = AglBoundingSphere::minimumBoundingSphere(mHull->vertices);
	AglVector3 c = AglVector3(0, 0, 0);
	for (unsigned int i = 0; i < mHull->vertices.size(); i++)
	{
		c += mHull->vertices[i];
	}
	c /= (float)mHull->vertices.size();

	float max = AglVector3::lengthSquared(mHull->vertices[0] - bs.position);

	for (unsigned int i = 0; i < mHull->vertices.size(); i++)
	{
		mHull->vertices[i] -= bs.position;
		if (AglVector3::lengthSquared(mHull->vertices[i]) > max)
			max = AglVector3::lengthSquared(mHull->vertices[i]);
	}
	float frac = 1.0f / sqrt(max);
	for (unsigned int i = 0; i < mHull->vertices.size(); i++)
	{
		mHull->vertices[i] *= frac;
	}

	mOBB = AglOBB::constructMinimum(mHull->vertices, mHull->indices);
}
ConvexHullShape::~ConvexHullShape()
{
	delete mHull;
}
vector<AglVector3> ConvexHullShape::GetVertices()
{
	return mHull->vertices;
}
vector<unsigned int>	ConvexHullShape::GetIndices()
{
	return mHull->indices;
}
AglOBB ConvexHullShape::GetOBB()
{
	return mOBB;
}
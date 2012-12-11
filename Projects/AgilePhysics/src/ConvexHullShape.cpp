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

	//Create the convex hull by using an iterative
	//hull creation solver.


	//Step 1: Create the startup hull using four of the random points
	//In rare cases these points could lie in the same plane or line
	//which causes the algorithm to fail. This can be solved by adding
	//an extra check for this.
	AglVector3 c = AglVector3(0, 0, 0);
	for (int i = 0; i < 4; i++)
	{
		mVertices.push_back(points.back());
		c += points.back();
		points.pop_back();
	}
	c *= 0.25f;

	//Step 1: ...cont. Create an initial triangle mesh.
	//Ensure that the triangles are correctly winded.
	AddCorrectlyWinded(0, 1, 2, mVertices, mIndices, c);
	AddCorrectlyWinded(0, 1, 3, mVertices, mIndices, c);
	AddCorrectlyWinded(0, 2, 3, mVertices, mIndices, c);
	AddCorrectlyWinded(1, 2, 3, mVertices, mIndices, c);

	while (points.size() > 0)
	{
		AddPoint(points.back(), mVertices, mIndices);
		points.pop_back();
	}

	//Move the shape so that the center lies at the origin
	//Normalize it such that the furthest point lies at 
	//a distance of 1.0f from the origin.
	AglBoundingSphere bs = AglBoundingSphere::minimumBoundingSphere(mVertices);
	c = AglVector3(0, 0, 0);
	for (unsigned int i = 0; i < mVertices.size(); i++)
	{
		c += mVertices[i];
	}
	c /= (float)mVertices.size();

	float max = AglVector3::lengthSquared(mVertices[0] - bs.position);

	for (unsigned int i = 0; i < mVertices.size(); i++)
	{
		mVertices[i] -= bs.position;
		if (AglVector3::lengthSquared(mVertices[i]) > max)
			max = AglVector3::lengthSquared(mVertices[i]);
	}
	float frac = 1.0f / sqrt(max);
	for (unsigned int i = 0; i < mVertices.size(); i++)
	{
		mVertices[i] *= frac;
	}

	mOBB = AglOBB::constructMinimum(mVertices, mIndices);
}
ConvexHullShape::~ConvexHullShape()
{
}
void ConvexHullShape::AddCorrectlyWinded(int pI1, int pI2, int pI3, vector<AglVector3> pPoints, vector<int>& pIndices, AglVector3 pReference)
{
	if (AglVector3::dotProduct(AglVector3::crossProduct(pPoints[pI2] - pPoints[pI1], pPoints[pI3] - pPoints[pI1]), pPoints[pI1] - pReference) > 0)
	{
		pIndices.push_back(pI1);
		pIndices.push_back(pI2);
		pIndices.push_back(pI3);
	}
	else
	{
		pIndices.push_back(pI1);
		pIndices.push_back(pI3);
		pIndices.push_back(pI2);
	}
}
void ConvexHullShape::AddPoint(AglVector3 pPoint, vector<AglVector3>& pP, vector<int>& pI)
{
	vector<int> seen;
	vector<pair<int,int>> edges;
	for (unsigned int i = 0; i < pI.size(); i+=3)
	{
		AglVector3 n = AglVector3::crossProduct(pP[pI[i+1]] - pP[pI[i]], pP[pI[i+2]] - pP[pI[i]]);
		if (AglVector3::dotProduct(pPoint - pP[pI[i]], n) >= 0)
		{
			seen.push_back(i);
			InsertUnique(edges, pair<int, int>(pI[i], pI[i+1]));
			InsertUnique(edges, pair<int, int>(pI[i+2], pI[i]));
			InsertUnique(edges, pair<int, int>(pI[i+1], pI[i+2]));
		}
	}
	for (int i = seen.size()-1; i >= 0; i--)
	{
		pI[seen[i]] = pI[pI.size()-3];
		pI[seen[i]+1] = pI[pI.size()-2];
		pI[seen[i]+2] = pI[pI.size()-1];
		pI.pop_back();
		pI.pop_back();
		pI.pop_back();
	}
	if (edges.size() > 0)
	{
		for (unsigned int i = 0; i < edges.size(); i++)
		{
			pI.push_back(edges[i].first);
			pI.push_back(edges[i].second);
			pI.push_back(pP.size());
		}
		pP.push_back(pPoint);
	}
}
void ConvexHullShape::InsertUnique(vector<pair<int,int>>& pEdges, pair<int, int> pNew)
{
	for (unsigned int i = 0; i < pEdges.size(); i++)
	{
		if (pEdges[i].first == pNew.first && pEdges[i].second == pNew.second)
		{
			pEdges[i] = pEdges.back();
			pEdges.pop_back();
			return;
		}
		if (pEdges[i].second == pNew.first && pEdges[i].first == pNew.second) //I don't think this will ever happen if I maintain correct winding
		{
			pEdges[i] = pEdges.back();
			pEdges.pop_back();
			return;
		}
	}
	pEdges.push_back(pNew);
}
vector<AglVector3> ConvexHullShape::GetVertices()
{
	return mVertices;
}
vector<int>	ConvexHullShape::GetIndices()
{
	return mIndices;
}
AglOBB ConvexHullShape::GetOBB()
{
	return mOBB;
}
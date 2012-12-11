#ifndef CONVEXHULLSHAPE_H
#define CONVEXHULLSHAPE_H

#include "PhyUtility.h"
#include "AglBoundingSphere.h"
#include "AglOBB.h"

class ConvexHullShape
{
private:
	vector<AglVector3> mVertices;
	vector<int>		mIndices;

	AglOBB			mOBB;
private:
	//Adds a triangle to a set of triangles with correct winding.
	//Correct winding is achieved when the triangle faces away from the reference point.
	void AddCorrectlyWinded(int pI1, int pI2, int pI3, vector<AglVector3> pPoints, vector<int>& pIndices, AglVector3 pReference);

	void AddPoint(AglVector3 pPoint, vector<AglVector3>& pP, vector<int>& pI);

	void InsertUnique(vector<pair<int,int>>& pEdges, pair<int, int> pNew);
public:
	//Creates a Convex Hull using a certain number of randomly
	//distributed points over the unit sphere.
	ConvexHullShape(int);
	~ConvexHullShape();

	vector<AglVector3> GetVertices();
	vector<int>		GetIndices();

	AglOBB GetOBB();
};

#endif
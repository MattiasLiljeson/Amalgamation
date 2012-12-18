#ifndef CONVEXHULLSHAPE_H
#define CONVEXHULLSHAPE_H

#include "PhyUtility.h"
#include "AglBoundingSphere.h"
#include "AglOBB.h"
#include "AglHull.h"

class ConvexHullShape
{
private:
	AglHull* mHull;

	AglOBB			mOBB;

public:
	ConvexHullShape(int);
	~ConvexHullShape();

	vector<AglVector3>		GetVertices();
	vector<unsigned int>	GetIndices();

	AglOBB GetOBB();
};

#endif
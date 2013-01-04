#ifndef RIGIDBODYCONVEXHULL_H
#define RIGIDBODYCONVEXHULL_H

#include "RigidBody.h"
#include "ConvexHullShape.h"
#include "PhyUtility.h"

class RigidBodyConvexHull: public RigidBody
{
private:
	ConvexHullShape* mShape;
	float			 mSize;

	//Cached Exterior points - Must be updated each time position is updated.
	vector<AglVector3> mPointsWorld;

private:
	void CalculateInertiaTensor();
public:
	RigidBodyConvexHull(ConvexHullShape* pShape, AglVector3 pPosition, float pSize, float pMass, AglVector3 pVelocity, AglVector3 pAngularVelocity, bool pStatic);
	~RigidBodyConvexHull();
	RigidBodyType GetType();
	const vector<AglVector3>& GetExteriorPoints();
	void UpdatePosition(float pElapsedTime);
	float GetSize(){ return mSize; }
	AglMatrix GetSizeAsMatrix(){ return AglMatrix(mSize, 0, 0, 0, 0, mSize, 0, 0, 0, 0, mSize, 0, 0, 0, 0, 1); }
	vector<PhyPlane> GetPlanes();
	ConvexHullShape* getShape(){ return mShape; }
	AglVector3 GetLocalCenterOfMass(){ return GetPosition(); }
	AglOBB GetOBB(){ return mShape->GetOBB(); }
};

#endif
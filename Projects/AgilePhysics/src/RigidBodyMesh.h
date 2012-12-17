// =======================================================================================
//                                      RigidBodyMesh
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # RigidBodyMesh
/// Detailed description.....
/// Created on: 17-12-2012 
///---------------------------------------------------------------------------------------
#ifndef RIGIDBODYMESH_H
#define RIGIDBODYMESH_H

#include "RigidBody.h"
#include <AglOBB.h>
#include <AglLooseBspTree.h>
#include <AglInteriorSphereGrid.h>
#include "GJKSolver.h"

class RigidBodySphere;

class RigidBodyMesh: public RigidBody
{
private:
	AglOBB mOBB;
	AglLooseBspTree* mBSPTree;
	AglInteriorSphereGrid* mSphereGrid;
private:
	void CalculateInertiaTensor();
public:
	RigidBodyMesh(AglVector3 pPosition, AglOBB pOBB, AglBoundingSphere pBoundingSphere, AglLooseBspTree* pBSPTree = NULL,
					AglInteriorSphereGrid* pSphereGrid = NULL);
	virtual ~RigidBodyMesh();
	RigidBodyType GetType();
	AglVector3 GetLocalCenterOfMass(){ return GetOBB().world.GetTranslation(); }

	AglOBB GetOBB()
	{
		AglOBB obb = mOBB;
		obb.world *= GetWorld();
		return obb; 
	}
	virtual AglBoundingSphere GetBoundingSphere() const
	{
		AglVector3 newPos = mBoundingSphere.position;
		newPos.transform(GetWorld());

		AglBoundingSphere bs;
		bs.radius = mBoundingSphere.radius;
		bs.position = newPos;
		return bs; 
	}
	bool EvaluateSphere(RigidBodySphere* pSphere, EPACollisionData* pData);
	bool Evaluate(AglVector3 p_c, float p_r, EPACollisionData* pData);
};

#endif // RIGIDBODYMESH_H
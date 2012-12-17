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
	AglVector3 GetLocalCenterOfMass(){ return GetPosition(); }

	AglOBB GetOBB()
	{
		AglOBB obb = mOBB;
		obb.world *= GetWorld();
		return obb; 
	}
	virtual AglBoundingSphere GetBoundingSphere() const
	{
		AglBoundingSphere bs;
		bs.radius = mBoundingSphere.radius;
		bs.position = GetPosition() + mBoundingSphere.position;
		return bs; 
	}
};

#endif // RIGIDBODYMESH_H
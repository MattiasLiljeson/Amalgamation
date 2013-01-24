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
class RigidBodyBox;

class RigidBodyMesh: public RigidBody
{
private:
	AglOBB mOBB;
	AglLooseBspTree* mBSPTree;
	AglInteriorSphereGrid* mSphereGrid;
	vector<pair<float, AglVector3>> normalList;
	int ind;
	AglVector3 mSize;
private:
	void CalculateInertiaTensor();
	bool Evaluate(AglVector3 p_c, float p_r, vector<EPACollisionData>& pData);
	bool Evaluate(vector<AglVector3> p_points, AglVector3 p_u1, AglVector3 p_u2, AglVector3 p_u3, vector<AglVector3>& pData);
	bool Evaluate(AglVector3* p_axes, AglBspNode* p_n1, AglBspNode* p_n2, RigidBodyMesh* p_other, vector<AglVector3>& p_triangles);

public:
	RigidBodyMesh(AglVector3 pPosition, AglOBB pOBB, AglBoundingSphere pBoundingSphere, AglLooseBspTree* pBSPTree = NULL,
					AglInteriorSphereGrid* pSphereGrid = NULL, bool pImpulseEnabled = true);
	RigidBodyMesh(AglMatrix pWorld, AglOBB pOBB, AglBoundingSphere pBoundingSphere, AglVector3 pSize, AglLooseBspTree* pBSPTree = NULL);

	virtual ~RigidBodyMesh();
	RigidBodyType GetType();
	AglVector3 GetLocalCenterOfMass(){ return GetOBB().world.GetTranslation(); }

	AglOBB GetOBB()
	{
		AglOBB obb = mOBB;
		obb.size *= mSize;
		obb.world *= GetWorld();
		return obb; 
	}
	virtual AglBoundingSphere GetBoundingSphere() const
	{
		AglVector3 newPos = mBoundingSphere.position;
		newPos.transform(GetWorld());

		AglBoundingSphere bs;
		bs.radius = mBoundingSphere.radius * max(mSize.x, max(mSize.y, mSize.z));
		bs.position = newPos;
		return bs; 
	}
	bool EvaluateSphere(RigidBodySphere* pSphere, vector<EPACollisionData>& pData);
	bool EvaluateBox(RigidBodyBox* pBox, vector<AglVector3>& pData);
	bool EvaluateMesh(RigidBodyMesh* pMesh, vector<AglVector3>& pData);
	vector<pair<float, AglVector3>> GetNormalList(){ return normalList; }
	virtual void UpdateVelocity(float pElapsedTime)
	{
		RigidBody::UpdateVelocity(pElapsedTime);
		ind++;
	}
	AglVector3 GetSize()
	{
		return mSize;
	}
};

#endif // RIGIDBODYMESH_H
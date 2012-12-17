#include "RigidBodyMesh.h"

void RigidBodyMesh::CalculateInertiaTensor()
{
	AglVector3 s = mOBB.size;
	float xfactor = (4 * s[0] * s[0]);
	float yfactor = (4 * s[1] * s[1]);
	float zfactor = (4 * s[2] * s[2]);
	float x = (yfactor + zfactor) * GetMass() / 12.0f;
	float y = (xfactor + zfactor) * GetMass() / 12.0f;
	float z = (xfactor + yfactor) * GetMass() / 12.0f;
	AglMatrix i(x, 0, 0, 0, 0, y, 0, 0, 0, 0, z, 0, 0, 0, 0, 1);

	AglMatrix w = mOBB.world;
	w.SetTranslation(AglVector3(0, 0, 0));

	i = i * w;
	SetInertiaTensor(i);
}

//How should transforms be handled. Right now obb, bounding sphere, bsp tree and sphere grid are all
//given in local space but not relative to the mesh.
RigidBodyMesh::RigidBodyMesh(AglVector3 pPosition, AglOBB pOBB, AglBoundingSphere pBoundingSphere, AglLooseBspTree* pBSPTree,
							  AglInteriorSphereGrid* pSphereGrid) : RigidBody(pPosition)
{
	mOBB = pOBB;
	mBoundingSphere = pBoundingSphere; 
	mBSPTree = pBSPTree;
	mSphereGrid = pSphereGrid;
	CalculateInertiaTensor();
}

RigidBodyMesh::~RigidBodyMesh()
{

}

RigidBodyType RigidBodyMesh::GetType()
{
	return MESH;
}

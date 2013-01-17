// =======================================================================================
//                                      Octree
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # Octree
/// Detailed description.....
/// Created on: 16-1-2013 
///---------------------------------------------------------------------------------------
#ifndef OCTREE_H
#define OCTREE_H

#include <AglVector3.h>
#include "RigidBody.h"
#include "CollisionManager.h"

struct OctreeObject
{
	RigidBody* body;
	OctreeObject* next;

	OctreeObject(RigidBody* pBody, OctreeObject* pNext = NULL)
	{
		body = pBody;
		next = pNext;
	}
};

struct OctreeNode
{
	OctreeNode* Children[8];
	OctreeObject* bodies;
	AglVector3 Min;
	AglVector3 Max;

	OctreeNode(AglVector3 pMin, AglVector3 pMax)
	{
		Min = pMin;
		Max = pMax;
		Children[0] = NULL;
		Children[1] = NULL;
		Children[2] = NULL;
		Children[3] = NULL;
		Children[4] = NULL;
		Children[5] = NULL;
		Children[6] = NULL;
		Children[7] = NULL;
		bodies		= NULL;
	}
};

class Octree
{
private:
	OctreeNode* m_root;
private:
	OctreeNode* CreateNode(unsigned p_depthLeft, AglVector3 p_min, AglVector3 p_max);
	void Insert(RigidBody* pBody, OctreeNode* pNode);
	void Query(RigidBody* pBody, OctreeNode* pNode, vector<PhyCollisionData>& pCollisions); 
	void Delete(OctreeNode* pNode);
public:
	Octree(unsigned int p_depth, AglVector3 pMin, AglVector3 pMax);

	void Insert(RigidBody* pBody);
	vector<PhyCollisionData> Query(RigidBody* pBody);
	virtual ~Octree();
};

#endif // OCTREE_H
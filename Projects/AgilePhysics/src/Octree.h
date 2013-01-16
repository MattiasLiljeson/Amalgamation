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

struct OctreeNode
{
	OctreeNode* Children[8];
	AglVector3 Min;
	AglVector3 Max;
};

class Octree
{
private:
	OctreeNode* m_parent;
private:
	OctreeNode* createNode(unsigned p_depthLeft, OctreeNode* p_parent);
public:
	Octree(unsigned int p_depth);
	virtual ~Octree();
};

#endif // OCTREE_H
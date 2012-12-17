#ifndef AGLLOOSEBSPTREE_H
#define AGLLOOSEBSPTREE_H

#include "AglVector3.h"
#include <vector>
#include <algorithm>

using namespace std;

// =======================================================================================
//                                      AglBspNode
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Defines a node in a bsp tree
///        
/// # AglLooseBspTree
/// Created on: 27-11-2012 
///---------------------------------------------------------------------------------------
struct AglBspNode
{
	AglVector3	minPoint;
	AglVector3	maxPoint;
	int			leftChild;
	int			rightChild;
	int			triangleID;

	///-----------------------------------------------------------------------------------
	/// Constructor
	///-----------------------------------------------------------------------------------
	AglBspNode();
};

struct AglBspSphere
{
	AglVector3	center;
	float		radius;
};

struct AglBspTriangle
{
	unsigned int	index;
	AglVector3		vertices[3];
	AglVector3		center;
	AglVector3		normal;
	AglBspTriangle(unsigned int p_index, const vector<AglVector3>& p_vertices, const vector<unsigned int>& p_indices);
	bool operator<(const AglBspTriangle& pOther);

};


// =======================================================================================
//                                      AglLooseBspTree
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Defines a bounding volume hierarchy that iteratively subdivides geometry.
///        
/// # AglLooseBspTree
/// Created on: 27-11-2012 
///---------------------------------------------------------------------------------------

class AglLooseBspTree;

class AglLooseBspTreeConstructor
{
private:
	vector<AglBspNode>		m_nodes;
	unsigned int			m_root;
	vector<AglBspTriangle>	m_triangles;
	unsigned int			m_targetMesh;
private:
	int createNode(int p_start, int p_end, int p_index);
public:
	AglLooseBspTreeConstructor(unsigned int p_targetMesh, vector<AglVector3> p_vertices, vector<unsigned int> p_indices);
	virtual ~AglLooseBspTreeConstructor();

	vector<AglBspTriangle> evaluate(AglVector3 p_c, float p_r);
	AglLooseBspTree* createTree();
};

struct AglLooseBspTreeHeader
{
	unsigned int targetMesh;
	unsigned int root;
	unsigned int nodeCount;
	unsigned int triangleCount;
};

class AglLooseBspTree
{
private:
	AglLooseBspTreeHeader	m_header;
	unsigned int*			m_triangles;
	AglVector3*				m_triangles2;
	AglBspNode*				m_nodes;
public:
	AglLooseBspTree(vector<AglBspNode> p_nodes, unsigned int p_root,
						vector<AglBspTriangle> p_triangles, 
							unsigned int p_targetMesh);

	AglLooseBspTree(AglLooseBspTreeHeader p_header, unsigned int* p_triangles,
						AglBspNode* p_nodes);

	virtual ~AglLooseBspTree();

	bool evaluate(AglVector3 p_c, float p_r);

	///-----------------------------------------------------------------------------------
	/// Get the header of the bsp tree
	/// \return The header
	///-----------------------------------------------------------------------------------
	AglLooseBspTreeHeader	getHeader();

	///-----------------------------------------------------------------------------------
	/// Get the list of triangles from the bsp tree
	/// \return List of triangles
	///-----------------------------------------------------------------------------------
	unsigned int*			getTriangles();

	///-----------------------------------------------------------------------------------
	/// Get the list of nodes from the bsp tree
	/// \return List of nodes
	///-----------------------------------------------------------------------------------
	AglBspNode*				getNodes();

	AglVector3*				getTriangles2(){ return m_triangles2; }
};

#endif
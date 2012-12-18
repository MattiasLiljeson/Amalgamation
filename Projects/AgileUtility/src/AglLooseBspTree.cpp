#include "AglLooseBspTree.h"
#include "AglBoundingSphere.h"

AglBspNode::AglBspNode()
{
	leftChild = -1;
	rightChild = -1;
	triangleID = -1;
	minPoint = AglVector3();
	maxPoint = AglVector3();
}

AglBspTriangle::AglBspTriangle(unsigned int p_index, const vector<AglVector3>& p_vertices, const vector<unsigned int>& p_indices)
{
	index = p_index;
	float factor = 1.0f / 3.0f;

	vertices[0] = p_vertices[p_indices[index*3]];
	vertices[1] = p_vertices[p_indices[index*3+1]];
	vertices[2] = p_vertices[p_indices[index*3+2]];

	center = vertices[0] + vertices[1] + vertices[2];
	center *= factor;
	normal = AglVector3(1, 0, 0);
}
bool AglBspTriangle::operator<(const AglBspTriangle& pOther)
{
	float dot1 = AglVector3::dotProduct(center, normal);
	float dot2 = AglVector3::dotProduct(pOther.center, pOther.normal);
	return dot1 < dot2;
}

AglLooseBspTreeConstructor::AglLooseBspTreeConstructor(unsigned int p_targetMesh, vector<AglVector3> p_vertices, vector<unsigned int> p_indices)
{
	int count = 1;
	while (count < (int)(p_indices.size() / 3))
	{
		count *= 2;
	}
	count += count-1;
	m_nodes = vector<AglBspNode>(count);
	m_root = 0;
	m_targetMesh = p_targetMesh;

	for (unsigned int i = 0; i < p_indices.size() / 3; i++)
	{
		AglBspTriangle t(i, p_vertices, p_indices);
		m_triangles.push_back(t);
	}
	m_root = createNode(0, m_triangles.size(), 0);
}

AglLooseBspTreeConstructor::~AglLooseBspTreeConstructor()
{

}

int AglLooseBspTreeConstructor::createNode(int p_start, int p_end, int p_index)
{
	//Find the minimum AABB enclosing the triangle set
	AglVector3 minP, maxP;
	minP = maxP = m_triangles[p_start].vertices[0]; //?
	AglBspTriangle t = m_triangles[p_start];
	for (int i = p_start; i < p_end; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			AglVector3 p = m_triangles[i].vertices[j]; //?
			minP = AglVector3(min(minP[0], p[0]), min(minP[1], p[1]), min(minP[2], p[2]));
			maxP = AglVector3(max(maxP[0], p[0]), max(maxP[1], p[1]), max(maxP[2], p[2]));
		}
	}

	//Find the longest axis of this AABB
	AglVector3 diff = maxP - minP;
	AglVector3 norm = AglVector3(1, 0, 0);
	if (diff[0] > diff[1] && diff[0] > diff[2])
		norm = AglVector3(1, 0, 0);
	else if (diff[1] > diff[2])
	{
		norm = AglVector3(0, 1, 0);
	}
	else
	{
		norm = AglVector3(0, 0, 1);
	}
	for (unsigned int i = 0; i < m_triangles.size(); i++)
	{
		m_triangles[i].normal = norm;
	}

	//Sort the geometry
	sort(m_triangles.begin() + p_start, m_triangles.begin() + p_end);

	AglBspNode n;
	n.minPoint = minP;
	n.maxPoint = maxP;

	m_nodes[p_index] = n;

	if (p_end - p_start > 1)
	{
		//Intermediate Node
		int mid = (int)((p_end + p_start) * 0.5f);
		m_nodes[p_index].leftChild = createNode(p_start, mid, 2*p_index+1);
		m_nodes[p_index].rightChild = createNode(mid, p_end, 2*p_index+2); 
		m_nodes[p_index].triangleID = -1;
	}
	else
	{
		//Leaf Node
		m_nodes[p_index].leftChild = -1;
		m_nodes[p_index].rightChild = -1;
		m_nodes[p_index].triangleID = p_start;
	}
	return p_index;
}

vector<AglBspTriangle> AglLooseBspTreeConstructor::evaluate(AglVector3 p_c, float p_r)
{
	vector<AglBspNode> toEvaluate;
	toEvaluate.push_back(m_nodes[0]);

	vector<AglBspTriangle> tris;
	while (toEvaluate.size() > 0)
	{
		AglBspNode curr = toEvaluate.back();
		toEvaluate.pop_back();
		AglVector3 point = (curr.maxPoint + curr.minPoint)*0.5f;
		AglVector3 size = (curr.maxPoint - curr.minPoint)*0.5f;
		AglVector3 dir = p_c - point;

		//Axis 1
		point[0] += max(min(dir[0], size[0]), -size[0]);

		//Axis 2
		point[1] += max(min(dir[1], size[1]), -size[1]);

		//Axis 3
		point[2] += max(min(dir[2], size[2]), -size[2]);

		//Check the distance to this closest point
		if (AglVector3::lengthSquared(p_c - point) < p_r * p_r)
		{
			if (curr.leftChild >= 0)
			{
				toEvaluate.push_back(m_nodes[curr.leftChild]);
				toEvaluate.push_back(m_nodes[curr.rightChild]);
			}
			else if (curr.triangleID >= 0)
			{
				tris.push_back(m_triangles[curr.triangleID]);
			}
		}
	}
	return vector<AglBspTriangle>();
}
AglLooseBspTree* AglLooseBspTreeConstructor::createTree()
{
	AglLooseBspTree* newTree = new AglLooseBspTree(m_nodes, m_root, 
													m_triangles, m_targetMesh);
	return newTree;
}

AglLooseBspTree::AglLooseBspTree(vector<AglBspNode> p_nodes, unsigned int p_root,
	vector<AglBspTriangle> p_triangles, 
	unsigned int p_targetMesh)
{
	m_header.nodeCount = p_nodes.size();
	m_header.root = p_root;
	m_header.targetMesh = p_targetMesh;
	m_header.triangleCount = p_triangles.size();

	m_nodes = new AglBspNode[m_header.nodeCount];
	for (unsigned int i = 0; i < m_header.nodeCount; i++)
	{
		m_nodes[i] = p_nodes[i];
	}
	m_triangles = new unsigned int[m_header.triangleCount];
	m_triangles2 = new AglVector3[m_header.triangleCount*3];
	for (unsigned int i = 0; i < m_header.triangleCount; i++)
	{
		m_triangles[i] = p_triangles[i].index;
		m_triangles2[i*3] = p_triangles[i].vertices[0];
		m_triangles2[i*3+1] = p_triangles[i].vertices[1];
		m_triangles2[i*3+2] = p_triangles[i].vertices[2];
	}
}
AglLooseBspTree::AglLooseBspTree(AglLooseBspTreeHeader p_header, unsigned int* p_triangles,
	AglBspNode* p_nodes)
{
	m_header = p_header;
	m_triangles = p_triangles;
	m_nodes = p_nodes;
}
AglLooseBspTree::~AglLooseBspTree()
{
	delete[] m_nodes;
	delete[] m_triangles;
}
bool AglLooseBspTree::evaluate(AglVector3 p_c, float p_r)
{
	vector<AglBspNode> toEvaluate;
	toEvaluate.push_back(m_nodes[0]);

	vector<AglVector3> points(3);
	while (toEvaluate.size() > 0)
	{
		AglBspNode curr = toEvaluate.back();
		toEvaluate.pop_back();
		AglVector3 point = (curr.maxPoint + curr.minPoint)*0.5f;
		AglVector3 size = (curr.maxPoint - curr.minPoint)*0.5f;
		AglVector3 dir = p_c - point;

		//Axis 1
		point[0] += max(min(dir[0], size[0]), -size[0]);

		//Axis 2
		point[1] += max(min(dir[1], size[1]), -size[1]);

		//Axis 3
		point[2] += max(min(dir[2], size[2]), -size[2]);

		//Check the distance to this closest point
		if (AglVector3::lengthSquared(p_c - point) < p_r * p_r)
		{
			if (curr.leftChild >= 0)
			{
				toEvaluate.push_back(m_nodes[curr.leftChild]);
				toEvaluate.push_back(m_nodes[curr.rightChild]);
			}
			else if (curr.triangleID >= 0)
			{
				return true;
			}
		}
	}
	return false;
}

AglLooseBspTreeHeader AglLooseBspTree::getHeader()
{
	return m_header;
}
unsigned int* AglLooseBspTree::getTriangles()
{
	return m_triangles;
}
AglBspNode* AglLooseBspTree::getNodes()
{
	return m_nodes;
}
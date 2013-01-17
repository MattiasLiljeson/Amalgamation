#include "Octree.h"

Octree::Octree(unsigned int p_depth, AglVector3 pMin, AglVector3 pMax)
{
	m_root = CreateNode(p_depth, pMin, pMax);
}
Octree::~Octree()
{
	Delete(m_root);
}
OctreeNode* Octree::CreateNode(unsigned p_depthLeft, AglVector3 p_min, AglVector3 p_max)
{
	OctreeNode* n = new OctreeNode(p_min, p_max);
	if (p_depthLeft > 0)
	{
		AglVector3 o = (p_max-p_min)*0.5f;
		unsigned int d = p_depthLeft-1;

		n->Children[0] = CreateNode(d, p_min, p_min+o);
		n->Children[1] = CreateNode(d, p_min + AglVector3(o.x, 0, 0), p_min + o + AglVector3(o.x, 0, 0));
		n->Children[2] = CreateNode(d, p_min + AglVector3(0, o.y, 0), p_min + o + AglVector3(0, o.y, 0));
		n->Children[3] = CreateNode(d, p_min + AglVector3(0, 0, o.z), p_min + o + AglVector3(0, 0, o.z));
		n->Children[4] = CreateNode(d, p_min + AglVector3(o.x, o.y, 0), p_min + o + AglVector3(o.x, o.y, 0));
		n->Children[5] = CreateNode(d, p_min + AglVector3(0, o.y, o.z), p_min + o + AglVector3(0, o.y, o.z));
		n->Children[6] = CreateNode(d, p_min + AglVector3(o.x, 0, o.z), p_min + o + AglVector3(o.x, 0, o.z));
		n->Children[7] = CreateNode(d, p_min+o, p_max);
	}
	return n;
}
void Octree::Insert(RigidBody* pBody)
{
	if (CheckCollision(pBody->GetBoundingSphere(), m_root->Min, m_root->Max))
	{
		Insert(pBody, m_root);
	}
}
void Octree::Insert(RigidBody* pBody, OctreeNode* pNode)
{
	if (pNode->Children[0] != NULL)
	{
		for (unsigned int i = 0; i < 8; i++)
		{
			OctreeNode* child = pNode->Children[i];
			if (CheckCollision(pBody->GetBoundingSphere(), child->Min, child->Max))
			{
				Insert(pBody, child);
			}
		}
	}
	else
	{
		OctreeObject* o = new OctreeObject(pBody, pNode->bodies);
		pNode->bodies = o;
	}
}
vector<PhyCollisionData> Octree::Query(RigidBody* pBody)
{
	vector<PhyCollisionData> cols;

	if (CheckCollision(pBody->GetBoundingSphere(), m_root->Min, m_root->Max))
	{
		Query(pBody, m_root, cols);
	}

	//Reset Collision Flags
	for (unsigned int i = 0; i < cols.size(); i++)
	{
		cols[i].Body1->SetCollisionFlag(false);
		cols[i].Body2->SetCollisionFlag(false);
	}
	return cols;
}
void Octree::Query(RigidBody* pBody, OctreeNode* pNode, vector<PhyCollisionData>& pCollisions)
{
	if (pNode->Children[0])
	{
		//Query Children
		for (unsigned int i = 0; i < 8; i++)
		{
			OctreeNode* child = pNode->Children[i];
			if (CheckCollision(pBody->GetBoundingSphere(), child->Min, child->Max))
			{
				Query(pBody, child, pCollisions);
			}
		}
	}
	OctreeObject* toCheck = pNode->bodies;
	while (toCheck)
	{
		PhyCollisionData colData;
		if (!toCheck->body->IsCollisionFlagged() && CheckCollision(pBody, toCheck->body, &colData))
		{
			pCollisions.push_back(colData);
			toCheck->body->SetCollisionFlag(true);
		}
		toCheck = toCheck->next;
	}
}
void Octree::Delete(OctreeNode* pNode)
{
	if (pNode->Children[0] != NULL)
	{
		for (unsigned int i = 0; i < 8; i++)
			Delete(pNode->Children[i]);
	}
	while (pNode->bodies)
	{
		OctreeObject* n = pNode->bodies->next;
		delete pNode->bodies;
		pNode->bodies = n;
	}
	delete pNode;
}
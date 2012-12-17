#include "RigidBodyMesh.h"
#include "RigidBodySphere.h"
#include "GJKSolver.h"

void RigidBodyMesh::CalculateInertiaTensor()
{
	/*AglVector3 s = mOBB.size;
	float xfactor = (4 * s[0] * s[0]);
	float yfactor = (4 * s[1] * s[1]);
	float zfactor = (4 * s[2] * s[2]);
	float x = (yfactor + zfactor) * GetMass() / 12.0f;
	float y = (xfactor + zfactor) * GetMass() / 12.0f;
	float z = (xfactor + yfactor) * GetMass() / 12.0f;
	AglMatrix i(x, 0, 0, 0, 0, y, 0, 0, 0, 0, z, 0, 0, 0, 0, 1);

	AglMatrix w = mOBB.world;
	w.SetTranslation(AglVector3(0, 0, 0));

	i = AglMatrix::transpose(w) * i * w;
	SetInertiaTensor(i);*/

	float inertia = 0.4f * GetMass() * mBoundingSphere.radius * mBoundingSphere.radius;
	SetInertiaTensor(AglMatrix(inertia, 0, 0, 0, 0, inertia, 0, 0, 0, 0, inertia, 0, 0, 0, 0, 1));
}

//How should transforms be handled. Right now obb, bounding sphere, bsp tree and sphere grid are all
//given in local space but not relative to the mesh.
RigidBodyMesh::RigidBodyMesh(AglVector3 pPosition, AglOBB pOBB, AglBoundingSphere pBoundingSphere, AglLooseBspTree* pBSPTree,
							  AglInteriorSphereGrid* pSphereGrid) : RigidBody(pPosition, 1, AglVector3(0, 0, 0), AglVector3(0, 0, 0), false)
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
bool RigidBodyMesh::EvaluateSphere(RigidBodySphere* pSphere, EPACollisionData* pData)
{
	AglVector3 pos = pSphere->GetPosition();
	AglMatrix invW = GetWorld();
	pos -= invW.GetTranslation();
	invW.SetTranslation(AglVector3(0, 0, 0));
	invW = AglMatrix::inverse(invW);
	pos.transform(invW);
	if (Evaluate(pos, pSphere->GetRadius(), pData))
	{
		pData->Normal.transformNormal(GetWorld());
		return true;
	}
	return false;
}

bool RigidBodyMesh::Evaluate(AglVector3 p_c, float p_r, EPACollisionData* pData)
{
	AglBspNode* m_nodes = mBSPTree->getNodes();
	AglVector3* m_triangles2 = mBSPTree->getTriangles2();

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
				//Perform a gjk test to determine if the objects intersect
				AglBoundingSphere bs;
				bs.position = p_c;
				bs.radius = p_r;
				points[0] = m_triangles2[curr.triangleID*3];
				points[1] = m_triangles2[curr.triangleID*3+1];
				points[2] = m_triangles2[curr.triangleID*3+2];

				//Prova att endast lägga till kollisionen och inte returnera
				//därigenom hitta rätt kollision
				if (gjkCheckCollision(points, bs, pData))
				{
					return true;
				}
			}
		}
	}
	return false;
}

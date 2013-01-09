#include "RigidBodyMesh.h"
#include "RigidBodySphere.h"
#include "GJKSolver.h"
#include "CollisionManager.h"
#include "RigidBodyBox.h"
#include <list>

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
							  AglInteriorSphereGrid* pSphereGrid, bool pImpulseEnabled) : RigidBody(pPosition, 1, AglVector3(0, 0, 0), AglVector3(0, 0, 0), false, false, pImpulseEnabled)
{
	mOBB = pOBB;
	mBoundingSphere = pBoundingSphere; 
	mBSPTree = pBSPTree;
	mSphereGrid = pSphereGrid;
	ind = 0;
	CalculateInertiaTensor();
}

RigidBodyMesh::~RigidBodyMesh()
{

}

RigidBodyType RigidBodyMesh::GetType()
{
	return MESH;
}
bool RigidBodyMesh::EvaluateSphere(RigidBodySphere* pSphere, vector<EPACollisionData>& pData)
{
	AglVector3 pos = pSphere->GetPosition();
	AglMatrix invW = GetWorld();
	pos -= invW.GetTranslation();
	invW.SetTranslation(AglVector3(0, 0, 0));
	invW = AglMatrix::inverse(invW);
	pos.transform(invW);
	if (Evaluate(pos, pSphere->GetRadius(), pData))
	{
		for (unsigned int i = 0; i < pData.size(); i++)
		{
			pData[i].Normal.transformNormal(GetWorld());
			pData[i].Normal.normalize();
			normalList.push_back(pair<float, AglVector3>(pData[i].Depth, pData[i].Normal));
		}
		return true;
	}
	return false;
}
bool RigidBodyMesh::EvaluateBox(RigidBodyBox* pBox, vector<AglVector3>& pData)
{
	AglMatrix a = GetWorld();
	AglMatrix aInv = a.inverse();
	AglMatrix b = pBox->GetWorld();
	b *= aInv;

	vector<AglVector3> corn;
	AglVector3 HalfSize = pBox->GetSizeAsVector3() / 2;
	corn.push_back(AglVector3(-HalfSize[0], -HalfSize[1], -HalfSize[2]));
	corn.push_back(AglVector3( HalfSize[0], -HalfSize[1], -HalfSize[2]));
	corn.push_back(AglVector3(-HalfSize[0],  HalfSize[1], -HalfSize[2]));
	corn.push_back(AglVector3(-HalfSize[0], -HalfSize[1],  HalfSize[2]));
	corn.push_back(AglVector3( HalfSize[0],  HalfSize[1], -HalfSize[2]));
	corn.push_back(AglVector3( HalfSize[0], -HalfSize[1],  HalfSize[2]));
	corn.push_back(AglVector3(-HalfSize[0],  HalfSize[1],  HalfSize[2]));
	corn.push_back(AglVector3( HalfSize[0],  HalfSize[1],  HalfSize[2]));
	for (int i = 0; i < 8; i++)
	{
		AglVec3Transform(corn[i], b);
	}
	return Evaluate(corn, b.GetRight(), b.GetUp(), b.GetForward(), pData);
}
bool RigidBodyMesh::EvaluateMesh(RigidBodyMesh* pMesh, vector<AglVector3>& pData)
{

	//Skip nine of the axes as they usually prevent few collisions.
	//This creates deeper traversal but evaluation of each node will
	//be faster.
	AglMatrix w1 = GetWorld();
	AglMatrix w2 = pMesh->GetWorld();
	AglVector3 axes[6];
	axes[0] = AglVector3(1, 0, 0);
	axes[1] = AglVector3(0, 1, 0);
	axes[2] = AglVector3(0, 0, 1);
	axes[3] = w2.GetRight();
	axes[4] = w2.GetUp();
	axes[5] = w2.GetForward();

	AglBspNode* n1 = mBSPTree->getNodes();
	AglBspNode* n2 = pMesh->mBSPTree->getNodes();

	return Evaluate(axes, n1, n2, pMesh, pData);
}

bool RigidBodyMesh::Evaluate(AglVector3 p_c, float p_r, vector<EPACollisionData>& pData)
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

				EPACollisionData data;
				if (CheckCollision(bs, points[0], points[1], points[2], &data))//  gjkCheckCollision(points, bs, pData))
				{
					if (data.Depth > 0)
					{
						AglVector3 c = (points[0] + points[1] + points[2]) / 3.0f;

						if (AglVector3::dotProduct(p_c - c, data.Normal) < 0)
							data.Normal = -data.Normal;

						pData.push_back(data);
						//return true;
					}
				}
			}
		}
	}
	if (pData.size() > 0)
		return true;
	return false;
}

bool RigidBodyMesh::Evaluate(vector<AglVector3> p_points, AglVector3 p_u1, AglVector3 p_u2, AglVector3 p_u3, vector<AglVector3>& pData)
{
	//Skip nine of the axes as they usually prevent few collisions.
	//This creates deeper traversal but evaluation of each node will
	//be faster.
	AglVector3 axes[6];
	axes[0] = p_u1;
	axes[1] = p_u2;
	axes[2] = p_u3;
	axes[3] = AglVector3(1, 0, 0);
	axes[4] = AglVector3(0, 1, 0);
	axes[5] = AglVector3(0, 0, 1);

	AglBspNode* m_nodes = mBSPTree->getNodes();
	vector<AglBspNode> toEvaluate;
	toEvaluate.push_back(m_nodes[0]);

	vector<AglVector3> points2(8);

	vector<AglVector3> points(3);

	AglVector3* m_triangles2 = mBSPTree->getTriangles2();

	while (toEvaluate.size() > 0)
	{
		AglBspNode curr = toEvaluate.back();
		toEvaluate.pop_back();

		points2[0] = curr.minPoint;
		points2[1] = AglVector3(curr.minPoint.x, curr.minPoint.y, curr.maxPoint.z);
		points2[2] = AglVector3(curr.minPoint.x, curr.maxPoint.y, curr.minPoint.z);
		points2[3] = AglVector3(curr.minPoint.x, curr.maxPoint.y, curr.maxPoint.z);
		points2[4] = AglVector3(curr.maxPoint.x, curr.minPoint.y, curr.minPoint.z);
		points2[5] = AglVector3(curr.maxPoint.x, curr.minPoint.y, curr.maxPoint.z);
		points2[6] = AglVector3(curr.maxPoint.x, curr.maxPoint.y, curr.minPoint.z);
		points2[7] = curr.maxPoint;

		bool col = true;
		for (int i = 0; i < 6; i++)
		{
			float overlap = OverlapAmount(p_points, points2, axes[i]);
			if (overlap <= 0)
			{
				col = false;
				break;
			}
		}
		if (col)
		{
			if (curr.leftChild >= 0)
			{
				toEvaluate.push_back(m_nodes[curr.leftChild]);
				toEvaluate.push_back(m_nodes[curr.rightChild]);
			}
			else if (curr.triangleID >= 0)
			{
				//Add the triangle to the list of collided triangles
				pData.push_back(m_triangles2[curr.triangleID*3]);
				pData.push_back(m_triangles2[curr.triangleID*3+1]);
				pData.push_back(m_triangles2[curr.triangleID*3+2]);
			}
		}

	}
	return pData.size() > 0;
}
bool RigidBodyMesh::Evaluate(AglVector3* p_axes, AglBspNode* p_n1, AglBspNode* p_n2, RigidBodyMesh* p_other, vector<AglVector3>& p_triangles)
{
	pair<int, int> toEvaluate[100];
	int currSize = 1;
	toEvaluate[0] = pair<int, int>(0, 0);

	AglVector3 points[8];
	AglVector3 points2[8];

	vector<AglVector3> tri1(3);
	vector<AglVector3> tri2(3);

	AglVector3* triangles1 = mBSPTree->getTriangles2();
	AglVector3* triangles2 = p_other->mBSPTree->getTriangles2();

	AglMatrix w1 = GetWorld();
	AglMatrix w2 = p_other->GetWorld();

	AglMatrix w = w2 * w1.inverse();

	int i1 = -1;
	int i2 = -1;
	AglBspNode n1, n2;
	while (currSize > 0)
	{
		pair<int, int> curr = toEvaluate[currSize-1];
		if (i1 != curr.first)
		{
			i1 = curr.first;
			n1 = p_n1[curr.first];

			points[0] = n1.minPoint;
			points[1] = AglVector3(n1.minPoint.x, n1.minPoint.y, n1.maxPoint.z);
			points[2] = AglVector3(n1.minPoint.x, n1.maxPoint.y, n1.minPoint.z);
			points[3] = AglVector3(n1.minPoint.x, n1.maxPoint.y, n1.maxPoint.z);
			points[4] = AglVector3(n1.maxPoint.x, n1.minPoint.y, n1.minPoint.z);
			points[5] = AglVector3(n1.maxPoint.x, n1.minPoint.y, n1.maxPoint.z);
			points[6] = AglVector3(n1.maxPoint.x, n1.maxPoint.y, n1.minPoint.z);
			points[7] = n1.maxPoint;	
		}
		if (i2 != curr.second)
		{
			i2 = curr.second;
			n2 = p_n2[curr.second];
			points2[0] = n2.minPoint;
			points2[1] = AglVector3(n2.minPoint.x, n2.minPoint.y, n2.maxPoint.z);
			points2[2] = AglVector3(n2.minPoint.x, n2.maxPoint.y, n2.minPoint.z);
			points2[3] = AglVector3(n2.minPoint.x, n2.maxPoint.y, n2.maxPoint.z);
			points2[4] = AglVector3(n2.maxPoint.x, n2.minPoint.y, n2.minPoint.z);
			points2[5] = AglVector3(n2.maxPoint.x, n2.minPoint.y, n2.maxPoint.z);
			points2[6] = AglVector3(n2.maxPoint.x, n2.maxPoint.y, n2.minPoint.z);
			points2[7] = n2.maxPoint;	

			points2[0].transform(w);
			points2[1].transform(w);
			points2[2].transform(w);
			points2[3].transform(w);
			points2[4].transform(w);
			points2[5].transform(w);
			points2[6].transform(w);
			points2[7].transform(w);
		}

		currSize--;

		bool col = true;
		for (int i = 0; i < 6; i++)
		{
			float overlap = OverlapAmount(points, points2, p_axes[i]);
			if (overlap <= 0)
			{
				col = false;
				break;
			}
		}
		if (col)
		{
			//If the nodes collide
			if (n1.triangleID == -1 && n2.triangleID == -1)
			{
				//None of them are childs
				if (AglVector3::lengthSquared(n1.maxPoint - n1.minPoint) > AglVector3::lengthSquared(n2.maxPoint - n2.minPoint))
				{
					toEvaluate[currSize++] = pair<int, int>(n1.leftChild, i2);
					toEvaluate[currSize++] = pair<int, int>(n1.rightChild, i2);
				}
				else
				{
					toEvaluate[currSize++] = pair<int, int>(i1, n2.leftChild);
					toEvaluate[currSize++] = pair<int, int>(i1, n2.rightChild);
				}
			}
			else if (n1.triangleID == -1)
			{
				//B2 is a child
				toEvaluate[currSize++] = pair<int, int>(n1.leftChild, i2);
				toEvaluate[currSize++] = pair<int, int>(n1.rightChild, i2);
			}
			else if (n2.triangleID == -1)
			{
				//B1 is a child
				toEvaluate[currSize++] = pair<int, int>(i1, n2.leftChild);
				toEvaluate[currSize++] = pair<int, int>(i1, n2.rightChild);
			}
			else
			{
				//Both are childs
				p_triangles.push_back(triangles1[n1.triangleID*3]);
				p_triangles.push_back(triangles1[n1.triangleID*3+1]);
				p_triangles.push_back(triangles1[n1.triangleID*3+2]);

				p_triangles.push_back(triangles2[n2.triangleID*3]);
				p_triangles.push_back(triangles2[n2.triangleID*3+1]);
				p_triangles.push_back(triangles2[n2.triangleID*3+2]);
			}
		}
	}
	return p_triangles.size() > 0;
}

/*bool RigidBodyMesh::Evaluate(AglVector3* p_axes, AglBspNode* p_n1, AglBspNode* p_n2, RigidBodyMesh* p_other, vector<AglVector3>& p_triangles)
{
	theGlobal = 0;
	vector<pair<AglBspNode, AglBspNode>> toEvaluate;
	toEvaluate.push_back(pair<AglBspNode, AglBspNode>(p_n1[0], p_n2[0]));

	vector<AglVector3> points(8);
	vector<AglVector3> points2(8);

	vector<AglVector3> tri1(3);
	vector<AglVector3> tri2(3);

	AglVector3* triangles1 = mBSPTree->getTriangles2();
	AglVector3* triangles2 = p_other->mBSPTree->getTriangles2();

	AglMatrix w1 = GetWorld();
	AglMatrix w2 = p_other->GetWorld();

	AglMatrix w = w2 * w1.inverse();

	while (toEvaluate.size() > 0)
	{
		AglBspNode n1 = toEvaluate.back().first;
		AglBspNode n2 = toEvaluate.back().second;
		toEvaluate.pop_back();

		points[0] = n1.minPoint;
		points[1] = AglVector3(n1.minPoint.x, n1.minPoint.y, n1.maxPoint.z);
		points[2] = AglVector3(n1.minPoint.x, n1.maxPoint.y, n1.minPoint.z);
		points[3] = AglVector3(n1.minPoint.x, n1.maxPoint.y, n1.maxPoint.z);
		points[4] = AglVector3(n1.maxPoint.x, n1.minPoint.y, n1.minPoint.z);
		points[5] = AglVector3(n1.maxPoint.x, n1.minPoint.y, n1.maxPoint.z);
		points[6] = AglVector3(n1.maxPoint.x, n1.maxPoint.y, n1.minPoint.z);
		points[7] = n1.maxPoint;											

		points2[0] = n2.minPoint;
		points2[1] = AglVector3(n2.minPoint.x, n2.minPoint.y, n2.maxPoint.z);
		points2[2] = AglVector3(n2.minPoint.x, n2.maxPoint.y, n2.minPoint.z);
		points2[3] = AglVector3(n2.minPoint.x, n2.maxPoint.y, n2.maxPoint.z);
		points2[4] = AglVector3(n2.maxPoint.x, n2.minPoint.y, n2.minPoint.z);
		points2[5] = AglVector3(n2.maxPoint.x, n2.minPoint.y, n2.maxPoint.z);
		points2[6] = AglVector3(n2.maxPoint.x, n2.maxPoint.y, n2.minPoint.z);
		points2[7] = n2.maxPoint;	

		for (unsigned int i = 0; i < 8; i++)
		{
			points2[i].transform(w);
		}

		bool col = true;
		for (int i = 0; i < 6; i++)
		{
			float overlap = OverlapAmount(points, points2, p_axes[i]);
			if (overlap <= 0)
			{
				col = false;
				break;
			}
		}
		if (col)
		{
			//If the nodes collide
			if (n1.triangleID == -1 && n2.triangleID == -1)
			{
				//None of them are childs
				if (AglVector3::lengthSquared(n1.maxPoint - n1.minPoint) > AglVector3::lengthSquared(n2.maxPoint - n2.minPoint))
				{
					toEvaluate.push_back(pair<AglBspNode, AglBspNode>(p_n1[n1.leftChild], n2));
					toEvaluate.push_back(pair<AglBspNode, AglBspNode>(p_n1[n1.rightChild], n2));
				}
				else
				{
					toEvaluate.push_back(pair<AglBspNode, AglBspNode>(n1, p_n2[n2.leftChild]));
					toEvaluate.push_back(pair<AglBspNode, AglBspNode>(n1, p_n2[n2.rightChild]));
				}
			}
			else if (n1.triangleID == -1)
			{
				//B2 is a child
				toEvaluate.push_back(pair<AglBspNode, AglBspNode>(p_n1[n1.leftChild], n2));
				toEvaluate.push_back(pair<AglBspNode, AglBspNode>(p_n1[n1.rightChild], n2));
			}
			else if (n2.triangleID == -1)
			{
				//B1 is a child
				toEvaluate.push_back(pair<AglBspNode, AglBspNode>(n1, p_n2[n2.leftChild]));
				toEvaluate.push_back(pair<AglBspNode, AglBspNode>(n1, p_n2[n2.rightChild]));
			}
			else
			{
				//Both are childs
				p_triangles.push_back(triangles1[n1.triangleID*3]);
				p_triangles.push_back(triangles1[n1.triangleID*3+1]);
				p_triangles.push_back(triangles1[n1.triangleID*3+2]);

				p_triangles.push_back(triangles2[n2.triangleID*3]);
				p_triangles.push_back(triangles2[n2.triangleID*3+1]);
				p_triangles.push_back(triangles2[n2.triangleID*3+2]);

				theGlobal++;
			}
		}
	}
	return p_triangles.size() > 0;
}*/



//Before vector optimization
/*bool RigidBodyMesh::Evaluate(AglVector3* p_axes, AglBspNode* p_n1, AglBspNode* p_n2, RigidBodyMesh* p_other, vector<AglVector3>& p_triangles)
{
	theGlobal = 0;
	vector<pair<int, int>> toEvaluate;
	toEvaluate.push_back(pair<int, int>(0, 0));

	vector<AglVector3> points(8);
	vector<AglVector3> points2(8);

	vector<AglVector3> tri1(3);
	vector<AglVector3> tri2(3);

	AglVector3* triangles1 = mBSPTree->getTriangles2();
	AglVector3* triangles2 = p_other->mBSPTree->getTriangles2();

	AglMatrix w1 = GetWorld();
	AglMatrix w2 = p_other->GetWorld();

	AglMatrix w = w2 * w1.inverse();

	int i1 = -1;
	int i2 = -1;
	AglBspNode n1, n2;
	while (toEvaluate.size() > 0)
	{
		pair<int, int> curr = toEvaluate.back();
		if (i1 != curr.first)
		{
			i1 = curr.first;
			n1 = p_n1[curr.first];

			points[0] = n1.minPoint;
			points[1] = AglVector3(n1.minPoint.x, n1.minPoint.y, n1.maxPoint.z);
			points[2] = AglVector3(n1.minPoint.x, n1.maxPoint.y, n1.minPoint.z);
			points[3] = AglVector3(n1.minPoint.x, n1.maxPoint.y, n1.maxPoint.z);
			points[4] = AglVector3(n1.maxPoint.x, n1.minPoint.y, n1.minPoint.z);
			points[5] = AglVector3(n1.maxPoint.x, n1.minPoint.y, n1.maxPoint.z);
			points[6] = AglVector3(n1.maxPoint.x, n1.maxPoint.y, n1.minPoint.z);
			points[7] = n1.maxPoint;	
		}
		if (i2 != curr.second)
		{
			i2 = curr.second;
			n2 = p_n2[curr.second];
			points2[0] = n2.minPoint;
			points2[1] = AglVector3(n2.minPoint.x, n2.minPoint.y, n2.maxPoint.z);
			points2[2] = AglVector3(n2.minPoint.x, n2.maxPoint.y, n2.minPoint.z);
			points2[3] = AglVector3(n2.minPoint.x, n2.maxPoint.y, n2.maxPoint.z);
			points2[4] = AglVector3(n2.maxPoint.x, n2.minPoint.y, n2.minPoint.z);
			points2[5] = AglVector3(n2.maxPoint.x, n2.minPoint.y, n2.maxPoint.z);
			points2[6] = AglVector3(n2.maxPoint.x, n2.maxPoint.y, n2.minPoint.z);
			points2[7] = n2.maxPoint;	

			for (unsigned int i = 0; i < 8; i++)
			{
				points2[i].transform(w);
			}
		}

		toEvaluate.pop_back();

		bool col = true;
		for (int i = 0; i < 6; i++)
		{
			float overlap = OverlapAmount(points, points2, p_axes[i]);
			if (overlap <= 0)
			{
				col = false;
				break;
			}
		}
		if (col)
		{
			//If the nodes collide
			if (n1.triangleID == -1 && n2.triangleID == -1)
			{
				//None of them are childs
				if (AglVector3::lengthSquared(n1.maxPoint - n1.minPoint) > AglVector3::lengthSquared(n2.maxPoint - n2.minPoint))
				{
					toEvaluate.push_back(pair<int, int>(n1.leftChild, i2));
					toEvaluate.push_back(pair<int, int>(n1.rightChild, i2));
				}
				else
				{
					toEvaluate.push_back(pair<int, int>(i1, n2.leftChild));
					toEvaluate.push_back(pair<int, int>(i1, n2.rightChild));
				}
			}
			else if (n1.triangleID == -1)
			{
				//B2 is a child
				toEvaluate.push_back(pair<int, int>(n1.leftChild, i2));
				toEvaluate.push_back(pair<int, int>(n1.rightChild, i2));
			}
			else if (n2.triangleID == -1)
			{
				//B1 is a child
				toEvaluate.push_back(pair<int, int>(i1, n2.leftChild));
				toEvaluate.push_back(pair<int, int>(i1, n2.rightChild));
			}
			else
			{
				//Both are childs
				p_triangles.push_back(triangles1[n1.triangleID*3]);
				p_triangles.push_back(triangles1[n1.triangleID*3+1]);
				p_triangles.push_back(triangles1[n1.triangleID*3+2]);

				p_triangles.push_back(triangles2[n2.triangleID*3]);
				p_triangles.push_back(triangles2[n2.triangleID*3+1]);
				p_triangles.push_back(triangles2[n2.triangleID*3+2]);

				theGlobal++;
			}
		}
	}
	return false;
	return p_triangles.size() > 0;
}*/
#include "CollisionManager.h"
#include "GJKSolver.h"

float theGlobal = 0;

bool CheckCollision(RigidBody* p_r1, RigidBody* p_r2, PhyCollisionData* p_data)
{
	//Do a bounding sphere check to provide early outs.
	if (!CheckCollision(p_r1->GetBoundingSphere(), p_r2->GetBoundingSphere()))
		return false;

	//Check types and do appropriate tests.
	if (p_r1->GetType() == SPHERE && p_r2->GetType() == SPHERE)
		return CheckCollision((RigidBodySphere*)p_r1, (RigidBodySphere*)p_r2, p_data);
	else if (p_r1->GetType() == SPHERE && p_r2->GetType() == BOX)
		return CheckCollision((RigidBodySphere*)p_r1, (RigidBodyBox*)p_r2, p_data);
	else if (p_r2->GetType() == SPHERE && p_r1->GetType() == BOX)
		return CheckCollision((RigidBodySphere*)p_r2, (RigidBodyBox*)p_r1, p_data);
	else if (p_r1->GetType() == BOX && p_r2->GetType() == BOX)
		return CheckCollision((RigidBodyBox*)p_r1, (RigidBodyBox*)p_r2, p_data);
	else if (p_r1->GetType() == CONVEXHULL && p_r2->GetType() == CONVEXHULL)
		return CheckCollision((RigidBodyConvexHull*)p_r1, (RigidBodyConvexHull*)p_r2, p_data);
	else if (p_r1->GetType() == BOX && p_r2->GetType() == CONVEXHULL)
		return CheckCollision((RigidBodyBox*)p_r1, (RigidBodyConvexHull*)p_r2, p_data);
	else if (p_r1->GetType() == CONVEXHULL && p_r2->GetType() == BOX)
		return CheckCollision((RigidBodyBox*)p_r2, (RigidBodyConvexHull*)p_r1, p_data);
	else if (p_r1->GetType() == CONVEXHULL && p_r2->GetType() == SPHERE)
		return CheckCollision((RigidBodySphere*)p_r2, (RigidBodyConvexHull*)p_r1, p_data);
	else if (p_r2->GetType() == CONVEXHULL && p_r1->GetType() == SPHERE)
		return CheckCollision((RigidBodySphere*)p_r1, (RigidBodyConvexHull*)p_r2, p_data);
	else if (p_r1->GetType() == SPHERE && p_r2->GetType() == MESH)
		return CheckCollision((RigidBodySphere*)p_r1, (RigidBodyMesh*)p_r2, p_data);
	else if (p_r1->GetType() == MESH && p_r2->GetType() == SPHERE)
		return CheckCollision((RigidBodySphere*)p_r2, (RigidBodyMesh*)p_r1, p_data);
	else if (p_r1->GetType() == BOX && p_r2->GetType() == MESH)
		return CheckCollision((RigidBodyBox*)p_r1, (RigidBodyMesh*)p_r2, p_data);
	else if (p_r1->GetType() == MESH && p_r2->GetType() == BOX)
		return CheckCollision((RigidBodyBox*)p_r2, (RigidBodyMesh*)p_r1, p_data);
	else if (p_r1->GetType() == MESH && p_r2->GetType() == MESH)
		return CheckCollision((RigidBodyMesh*)p_r1, (RigidBodyMesh*)p_r2, p_data);
	else
		return false;
}

//---------------------------------RAY COLLISIONS-----------------------------------------

bool CheckCollision(PhyRay p_ray, RigidBody* p_rigidBody, RayCollisionData* p_collisionData)
{
	//Check types and do appropriate tests.
	if (p_rigidBody->GetType() == BOX)
		return CheckCollision(p_ray, (RigidBodyBox*)p_rigidBody, p_collisionData);
	else if (p_rigidBody->GetType() == SPHERE)
		return CheckCollision((RigidBodySphere*)p_rigidBody, p_ray, p_collisionData);
	else if (p_rigidBody->GetType() == CONVEXHULL)
		return CheckCollision(p_ray, (RigidBodyConvexHull*)p_rigidBody, p_collisionData);
	return false;
}
bool CheckCollision(AglBoundingSphere p_sphere, RigidBody* p_rigidBody)
{
	if (p_rigidBody->GetType() == BOX)
		return CheckCollision(p_sphere, (RigidBodyBox*)p_rigidBody);
	else if (p_rigidBody->GetType() == SPHERE)
		return CheckCollision(p_sphere, p_rigidBody->GetBoundingSphere());
	else if (p_rigidBody->GetType() == CONVEXHULL)
		return CheckCollision(p_sphere, (RigidBodyConvexHull*)p_rigidBody);
	return false;
}
bool CheckCollision(AglBoundingSphere pS, PhyRay pR)
{
	//Sphere Info
	AglVector3 C = pS.position;
	float radius = pS.radius;
	
	//Sphere collision algorithm
	float b = AglVector3::dotProduct(pR.d, pR.o - C);
	float c = AglVector3::dotProduct(pR.o - C, pR.o - C) - radius*radius;
	if (b*b - c >= 0)
	{
		//If time of collision is positive
		float t = (b*b-c) - b*b;
		if (t > 0)
			return true;
	}
	return false;
}
bool CheckCollision(RigidBodySphere* p_sphere, PhyRay p_ray, RayCollisionData* p_collisionData)
{
	AglVector3 d = p_ray.d;
	AglVector3::normalize(d);
	//Sphere Info
	AglVector3 C = p_sphere->GetPosition();
	float radius = p_sphere->GetRadius();

	//Sphere collision algorithm
	float b = AglVector3::dotProduct(d, p_ray.o - C);
	float c = AglVector3::dotProduct(p_ray.o - C, p_ray.o - C) - radius*radius;
	if (b*b - c >= 0)
	{
		float t1 = -b - sqrt(b*b-c);
		float t2 = -b + sqrt(b*b-c);
		if (t2 > 0)
		{
			if (t1 > 0)
				p_collisionData->t = t1/AglVector3::length(p_ray.d);
			else
				p_collisionData->t = t2/AglVector3::length(p_ray.d);
			return true;
		}
	}
	return false;
}
bool CheckCollision(PhyRay pR, PhyTriangle pT)
{
	AglVector3 e1 = -pR.d;
	AglVector3 e2 = pT.p2-pT.p1;
	AglVector3 e3 = pT.p3-pT.p1;
	AglVector3 s  = pR.o - pT.p1;

	//Find the determinants for time, barycentric u coordinate, barycentric v coordinate and fraction
	float time = AglDet(s, e2, e3);
	float u = AglDet(e1, s, e3);
	float v = AglDet(e1, e2, s);
	float frac = AglDet(e1, e2, e3);
	//Calculate t_u_v
	AglVector3 t_u_v = AglVector3(time, u, v) * (1/frac);

	//Check if there is an intersection
	if (t_u_v.x > 0 && t_u_v.y >= 0 && t_u_v.z >= 0 && (t_u_v.y + t_u_v.z) <= 1)
	{
		//return t_u_v;
		return true;
	}
	return false;
}
bool CheckCollision(PhyRay pR, AglVector3 pMin, AglVector3 pMax, RayCollisionData* collisionData)
{
	AglVector3 invD = AglVector3(1.0f / pR.d.x, 1.0f / pR.d.y, 1.0f / pR.d.z);
	float t1;
	float t2;
	AglVector3 diffMax = pMax - pR.o;	
	//diffMax *= invD;
	diffMax = AglVector3(diffMax.x*invD.x, diffMax.y*invD.y, diffMax.z*invD.z);
	AglVector3 diffMin = pMin - pR.o;
	//diffMin *= invD;
	diffMin = AglVector3(diffMin.x*invD.x, diffMin.y*invD.y, diffMin.z*invD.z);
	
	t1 = min(diffMin.x, diffMax.x);
	t2 = max(diffMin.x, diffMax.x);
	
	t1 = max(t1, min(diffMin.y, diffMax.y));
	t2 = min(t2, max(diffMin.y, diffMax.y));
	
	t1 = max(t1, min(diffMin.z, diffMax.z));
	t2 = min(t2, max(diffMin.z, diffMax.z));
	
	if (t1 > t2 || t2 < 0)
	{
		return false;
	}
	collisionData->t = t1;
	return true;
}
bool CheckCollision(PhyRay pR, RigidBodyBox* pBox, RayCollisionData* collisionData)
{
	if (!CheckCollision(pBox->GetBoundingSphere(), pR))
		return false;
	AglMatrix wInv = pBox->GetWorld();
	wInv = AglMatrix::inverse(wInv);

	PhyRay nR;
	nR.o = pR.o; 
	AglVec3Transform(nR.o, wInv);
	nR.d = pR.d; 
	AglVec3TransformNormal(nR.d, wInv);

	AglVector3 s = pBox->GetSizeAsVector3()*0.5f;
	return CheckCollision(nR, -s, s, collisionData);
}
bool CheckCollision(PhyRay p_ray, RigidBodyConvexHull* p_hull, RayCollisionData* p_collisionData)
{
	if (!CheckCollision(p_hull->GetBoundingSphere(), p_ray))
		return false;
	vector<PhyPlane> planes = p_hull->GetPlanes();

	float first = 0;
	float last = FLT_MAX;
	for (unsigned int i = 0; i < planes.size(); i++)
	{
		float denominator = AglVector3::dotProduct(planes[i].N, p_ray.d);
		float dist = planes[i].D - AglVector3::dotProduct(planes[i].N, p_ray.o);
		if (denominator == 0)
		{
			if (dist > 0)
				return 0;
		}
		else
		{
			float t = dist / denominator;
			if (denominator < 0.0f)
			{
				if (t > first)
					first = t;
			}
			else
			{
				if (t < last)
					last = t;
			}
			if (first > last)
				return false;
		}

	}
	
	if (first > 0)
		p_collisionData->t = first;
	else
		p_collisionData->t = last;

	return true;
}

bool CheckCollision(const LineSegment& p_lineSegment, RigidBodySphere* p_sphere)
{
	AglVector3 d = p_lineSegment.p2 - p_lineSegment.p1;
	AglVector3::normalize(d);
	//Sphere Info
	AglVector3 C = p_sphere->GetPosition();
	float radius = p_sphere->GetRadius();

	//Sphere collision algorithm
	float b = AglVector3::dotProduct(d, p_lineSegment.p1 - C);
	float c = AglVector3::dotProduct(p_lineSegment.p1 - C, p_lineSegment.p1 - C) - radius*radius;
	if (b*b - c >= 0)
	{
		float t1 = -b - sqrt(b*b-c);
		float t2 = -b + sqrt(b*b-c);
		if (t2 > 0 && t1 * t1 < AglVector3::lengthSquared(p_lineSegment.p2-p_lineSegment.p1))
		{
			return true;
		}
	}
	return false;
}
bool CheckCollision(const LineSegment& p_lineSegment, AglVector3 p_min, AglVector3 p_max)
{
	AglVector3 d = p_lineSegment.p2 - p_lineSegment.p1;
	d.normalize();

	AglVector3 invD = AglVector3(1.0f / d.x, 1.0f / d.y, 1.0f / d.z);
	float t1;
	float t2;
	AglVector3 diffMax = p_max - p_lineSegment.p1;	
	//diffMax *= invD;
	diffMax = AglVector3(diffMax.x*invD.x, diffMax.y*invD.y, diffMax.z*invD.z);
	AglVector3 diffMin = p_min - p_lineSegment.p1;
	//diffMin *= invD;
	diffMin = AglVector3(diffMin.x*invD.x, diffMin.y*invD.y, diffMin.z*invD.z);

	t1 = min(diffMin.x, diffMax.x);
	t2 = max(diffMin.x, diffMax.x);

	t1 = max(t1, min(diffMin.y, diffMax.y));
	t2 = min(t2, max(diffMin.y, diffMax.y));

	t1 = max(t1, min(diffMin.z, diffMax.z));
	t2 = min(t2, max(diffMin.z, diffMax.z));

	if (t1 > t2 || t2 < 0 || t1 * t1 > AglVector3::lengthSquared(p_lineSegment.p2 - p_lineSegment.p1))
	{
		return false;
	}
	return true;
}
bool CheckCollision(const LineSegment& p_lineSegment, RigidBodyBox* p_box)
{
	AglMatrix wInv = p_box->GetWorld();
	wInv = AglMatrix::inverse(wInv);

	LineSegment l;
	l.p1 = p_lineSegment.p1;
	AglVec3Transform(l.p1, wInv);
	l.p2 = p_lineSegment.p2;
	AglVec3Transform(l.p2, wInv);

	AglVector3 s = p_box->GetSizeAsVector3()*0.5f;
	return CheckCollision(l, -s, s);
}

//---------------------------------BODY COLLISIONS----------------------------------------

bool CheckCollision(AglBoundingSphere pS1, AglBoundingSphere pS2)
{
	AglVector3 dir = pS2.position - pS1.position;
	float r = pS1.radius + pS2.radius;
    if (AglVector3::lengthSquared(dir) < r*r)
    {
        return true;
    }
    return false;
}
bool CheckCollision(RigidBodySphere* pS1, RigidBodySphere* pS2)
{
    AglVector3 dir = pS2->GetPosition() - pS1->GetPosition();
    float r = pS1->GetRadius() + pS2->GetRadius();
    if (AglVector3::lengthSquared(dir) < r*r)
    {
        return true;
    }
    return false;
}
bool CheckCollision(RigidBodySphere* pS1, RigidBodySphere* pS2, PhyCollisionData* pData)
{
    AglVector3 dir = pS2->GetPosition() - pS1->GetPosition();
    float r = pS1->GetRadius() + pS2->GetRadius();
    if (AglVector3::lengthSquared(dir) < r*r)
    {
        AglVector3::normalize(dir);
        pData->Body1 = pS1;
        pData->Body2 = pS2;
		pData->Contacts.push_back(pair<AglVector3, AglVector3>(pS1->GetPosition() + dir * pS1->GetRadius(), pS2->GetPosition() - dir * pS2->GetRadius()));
        return true;
    }
    return false;
}
bool CheckCollision(RigidBodySphere* pSphere, RigidBodyBox* pBox, PhyCollisionData* pData)
{
	//Find the closest point on the box to the sphere
	AglVector3 point = pBox->GetPosition();
	AglVector3 dir = pSphere->GetPosition() - point;

	vector<AglVector3> axes = pBox->GetAxesWorld();
	AglVector3 size = pBox->GetSizeAsVector3()*0.5f;

	//Axis 1
    float proj = AglVector3::dotProduct(dir, axes[0]);
	proj = max(min(proj, size.x), -size.x);
    point += axes[0] * proj;

	//Axis 2
	proj = AglVector3::dotProduct(dir, axes[1]);
	proj = max(min(proj, size.y), -size.y);
    point += axes[1] * proj;

	//Axis 3
	proj = AglVector3::dotProduct(dir, axes[2]);
	proj = max(min(proj, size.z), -size.z);
    point += axes[2] * proj;

	//Check the distance to this closest point
	if (AglVector3::lengthSquared(pSphere->GetPosition() - point) < pSphere->GetRadius() * pSphere->GetRadius())
    {
		pData->Body2 = pSphere;
		pData->Body1 = pBox;
		pair<AglVector3, AglVector3> contact;
		AglVector3 dir = point - pSphere->GetPosition();
		AglVector3::normalize(dir);
		contact.second = pSphere->GetPosition() + dir * pSphere->GetRadius();
		contact.first = point;
		pData->Contacts.push_back(contact);
		return true;
    }
    return false;
}

bool CheckCollision(AglBoundingSphere pSphere, RigidBodyBox* pBox)
{
	//Find the closest point on the box to the sphere
	AglVector3 point = pBox->GetPosition();
	AglVector3 dir = pSphere.position - point;

	vector<AglVector3> axes = pBox->GetAxesWorld();
	AglVector3 size = pBox->GetSizeAsVector3()*0.5f;

	//Axis 1
    float proj = AglVector3::dotProduct(dir, axes[0]);
	proj = max(min(proj, size.x), -size.x);
    point += axes[0] * proj;

	//Axis 2
	proj = AglVector3::dotProduct(dir, axes[1]);
	proj = max(min(proj, size.y), -size.y);
    point += axes[1] * proj;

	//Axis 3
	proj = AglVector3::dotProduct(dir, axes[2]);
	proj = max(min(proj, size.z), -size.z);
    point += axes[2] * proj;

	//Check the distance to this closest point
	if (AglVector3::lengthSquared(pSphere.position - point) < pSphere.radius * pSphere.radius)
    {
		return true;
    }
    return false;
}

bool CheckCollision(AglBoundingSphere p_sphere, AglOBB p_box)
{
	//Find the closest point on the box to the sphere
	AglVector3 point = p_box.world.GetTranslation();
	AglVector3 dir = p_sphere.position - point;

	vector<AglVector3> axes;
	axes.push_back(p_box.world.GetRight());
	axes.push_back(p_box.world.GetUp());
	axes.push_back(p_box.world.GetForward());
	AglVector3 size = p_box.size*0.5f;

	//Axis 1
	float proj = AglVector3::dotProduct(dir, axes[0]);
	proj = max(min(proj, size.x), -size.x);
	point += axes[0] * proj;

	//Axis 2
	proj = AglVector3::dotProduct(dir, axes[1]);
	proj = max(min(proj, size.y), -size.y);
	point += axes[1] * proj;

	//Axis 3
	proj = AglVector3::dotProduct(dir, axes[2]);
	proj = max(min(proj, size.z), -size.z);
	point += axes[2] * proj;

	//Check the distance to this closest point
	if (AglVector3::lengthSquared(p_sphere.position - point) < p_sphere.radius * p_sphere.radius)
	{
		return true;
	}
	return false;
}

bool CheckCollision(AglBoundingSphere pSphere, AglVector3 pMin, AglVector3 pMax)
{
	AglVector3 point = (pMax+pMin)*0.5f;
	AglVector3 size = (pMax-pMin)*0.5f;
	AglVector3 dir = pSphere.position - point;

	//Axis 1
	point[0] += max(min(dir[0], size.x), -size.x);

	//Axis 2
	point[1] += max(min(dir[1], size.y), -size.y);

	//Axis 3
	point[2] += max(min(dir[2], size.z), -size.z);

	//Check the distance to this closest point
	if (AglVector3::lengthSquared(pSphere.position - point) < pSphere.radius * pSphere.radius)
    {
		return true;
    }
    return false;
}
bool CheckCollision(RigidBodySphere* pSphere, RigidBodyConvexHull* pHull, PhyCollisionData* pData)
{
	AglBoundingSphere bs = pSphere->GetBoundingSphere();
	EPACollisionData EPAData;
	bool col = gjkCheckCollision(pHull->GetExteriorPoints(), bs, &EPAData);
	if (col)
	{	
		pData->Body2 = pSphere;
		pData->Body1 = pHull;
		pair<AglVector3, AglVector3> contact;
		AglVector3 dir = EPAData.Normal;
		AglVector3::normalize(dir);
		contact.second = pSphere->GetPosition() - dir * pSphere->GetRadius();
		contact.first = contact.second + dir * EPAData.Depth;
		pData->Contacts.push_back(contact);
		return true;
	}
	return false;
}
bool CheckCollision(AglBoundingSphere p_sphere, RigidBodyConvexHull* p_hull)
{
	EPACollisionData EPAData;
	bool col = gjkCheckCollision(p_hull->GetExteriorPoints(), p_sphere, &EPAData);
	if (col)
	{	
		return true;
	}
	return false;
}
bool CheckCollision(RigidBodyBox* pB1, RigidBodyBox* pB2)
{
    AglVector3 axes[15];
	vector<AglVector3> aaxes = pB1->GetAxesWorld();
	vector<AglVector3> baxes = pB2->GetAxesWorld();
    axes[0] = aaxes[0];
    axes[1] = aaxes[1];
    axes[2] = aaxes[2];
    axes[3] = baxes[0];
    axes[4] = baxes[1];
    axes[5] = baxes[2];
    axes[6] = AglVector3::crossProduct(aaxes[0], baxes[0]);
    axes[7] = AglVector3::crossProduct(aaxes[0], baxes[1]);
    axes[8] = AglVector3::crossProduct(aaxes[0], baxes[2]);
    axes[9] = AglVector3::crossProduct(aaxes[1], baxes[0]);
    axes[10] = AglVector3::crossProduct(aaxes[1], baxes[1]);
    axes[11] = AglVector3::crossProduct(aaxes[1], baxes[2]);
    axes[12] = AglVector3::crossProduct(aaxes[2], baxes[0]);
    axes[13] = AglVector3::crossProduct(aaxes[2], baxes[1]);
    axes[14] = AglVector3::crossProduct(aaxes[2], baxes[2]);

    float Penetration = FLT_MAX;
    AglVector3 Normal(0,0,0);

    for (int i = 0; i < 15; i++)
    {
        float overlap = OverlapAmount(pB1, pB2, axes[i]);
        if (overlap <= 0)
            return false;
        else if (overlap < Penetration)
        {
            Penetration = overlap;
            Normal = axes[i];
        }
    }
	return true;
}
bool CheckCollision(RigidBodyBox* pB1, RigidBodyBox* pB2, PhyCollisionData* pData)
{
	if (!CheckCollision(pB1->GetBoundingSphere(), pB2))
		return false;
	if (!CheckCollision(pB2->GetBoundingSphere(), pB1))
		return false;

	EPACollisionData EPAData;
	if (gjkCheckCollision(pB2->GetCorners(), pB1->GetCorners(), &EPAData))
	{
		AglVector3 Normal = EPAData.Normal;
		float Penetration = EPAData.Depth;

		if (AglVector3::dotProduct(pB2->GetPosition() - pB1->GetPosition(), Normal) < 0)
			Normal = -Normal;

		vector<AglVector3> apoints = GetHitPoints(pB1, Normal, Penetration);
		vector<AglVector3> bpoints = GetHitPoints(pB2, -Normal, Penetration);

		//Vertex-Vertex, Vertex-Edge, Vertex-Face, Edge-Edge, Edge-Face, Face-Face

		if (apoints.size() == 4 && bpoints.size() == 4)//Case: Face - Face
		{
			vector<AglVector3> points = FindOverlapQuads(apoints, bpoints, Normal, Penetration);
			for (unsigned int i = 0; i < points.size(); i++)
			{
				pData->Contacts.push_back(pair<AglVector3, AglVector3>(points[i] + Normal * Penetration / 2, points[i] - Normal * Penetration / 2));
			}
			pData->Body1 = pB1;
			pData->Body2 = pB2;
		}
		else if (apoints.size() == 4 && bpoints.size() == 2)//Case: Edge-Face
		{
			vector<AglVector3> points = FindOverlapEdgeQuad(bpoints[0], bpoints[1], apoints, -Normal, Penetration);
			for (unsigned int i = 0; i < points.size(); i++)
			{
				pData->Contacts.push_back(pair<AglVector3, AglVector3>(points[i] + Normal * Penetration / 2, points[i] - Normal * Penetration / 2));
			}
			pData->Body1 = pB1;
			pData->Body2 = pB2;
		}
		else if (apoints.size() == 2 && bpoints.size() == 4)//Case: Edge-Face
		{
			vector<AglVector3> points = FindOverlapEdgeQuad(apoints[0], apoints[1], bpoints, Normal, Penetration);
			for (unsigned int i = 0; i < points.size(); i++)
			{
				pData->Contacts.push_back(pair<AglVector3, AglVector3>(points[i] + Normal * Penetration / 2, points[i] - Normal * Penetration / 2));
			}
			pData->Body1 = pB1;
			pData->Body2 = pB2;
		}
		else if (apoints.size() == 2 && bpoints.size() == 2)//Case: Edge-Edge
		{
			//Do not ignore! This is vital Do a distance check between edges.
			float s1 = 0, s2 = 0;
			EdgeEdgeDistance(apoints[0], apoints[1], bpoints[0], bpoints[1], s1, s2);
			pData->Body1 = pB1;
			pData->Body2 = pB2;
			pData->Contacts.push_back(pair<AglVector3, AglVector3>(apoints[0] + (apoints[1] - apoints[0]) * s1, bpoints[0] + (bpoints[1] - bpoints[0]) * s2));
		}
		else if (apoints.size() == 2 && bpoints.size() == 1)//Case: Edge-Vertex
		{
			//Ignore this case
			return false;
		}
		else if (apoints.size() == 1 && bpoints.size() == 2)//Case: Edge-Vertex
		{
			//Ignore this case
			return false;
		}
		else if (apoints.size() == 1 && bpoints.size() == 4)//Case: Face-Vertex
		{
			pData->Contacts.push_back(pair<AglVector3, AglVector3>(apoints[0], apoints[0] - Normal * Penetration));
			pData->Body1 = pB1;
			pData->Body2 = pB2;
		}
		else if (apoints.size() == 4 && bpoints.size() == 1)//Case: Face-Vertex
		{
			pData->Contacts.push_back(pair<AglVector3, AglVector3>(bpoints[0] + Normal * Penetration, bpoints[0]));
			pData->Body1 = pB1;
			pData->Body2 = pB2;
		}
		else if (apoints.size() == 1 && bpoints.size() == 1)//Case: Vertex-Vertex
		{
			//Ignore this case
			return false;
		}
		else
		{
			return false;
		}
		return true;
	}
	return false;
}
bool CheckCollision(RigidBodyConvexHull* pR1, RigidBodyConvexHull* pR2, PhyCollisionData* pData)
{
	EPACollisionData EPAData;
	vector<AglVector3> r1p = pR1->GetExteriorPoints();
	vector<AglVector3> r2p = pR2->GetExteriorPoints();
	if (gjkCheckCollision(r1p, r2p, &EPAData))
	{
		AglVector3 Normal = EPAData.Normal;
		float Penetration = EPAData.Depth;

		if (AglVector3::dotProduct(pR2->GetPosition() - pR1->GetPosition(), Normal) < 0)
			Normal = -Normal;

		vector<AglVector3> apoints = GetHitPoints(r1p, Normal, Penetration);
		vector<AglVector3> bpoints = GetHitPoints(r2p, -Normal, Penetration);

		//Vertex-Vertex, Vertex-Edge, Vertex-Face, Edge-Edge, Edge-Face, Face-Face

		if (apoints.size() == 3 && bpoints.size() == 3)//Case: Face - Face (Convex hulls are triangle based as opposed to boxes)
		{
			vector<AglVector3> points = FindOverlapTriangles(apoints, bpoints, Normal, Penetration);
			for (unsigned int i = 0; i < points.size(); i++)
			{
				pData->Contacts.push_back(pair<AglVector3, AglVector3>(points[i] + Normal * Penetration / 2, points[i] - Normal * Penetration / 2));
			}
			pData->Body1 = pR1;
			pData->Body2 = pR2;
		}
		else if (apoints.size() == 3 && bpoints.size() == 2)//Case: Edge-Face
		{
			vector<AglVector3> points = FindOverlapEdgeTriangle(bpoints[0], bpoints[1], apoints, -Normal, Penetration);
			for (unsigned int i = 0; i < points.size(); i++)
			{
				pData->Contacts.push_back(pair<AglVector3, AglVector3>(points[i] + Normal * Penetration / 2, points[i] - Normal * Penetration / 2));
			}
			pData->Body1 = pR1;
			pData->Body2 = pR2;
		}
		else if (apoints.size() == 2 && bpoints.size() == 3)//Case: Edge-Face
		{
			vector<AglVector3> points = FindOverlapEdgeTriangle(apoints[0], apoints[1], bpoints, Normal, Penetration);
			for (unsigned int i = 0; i < points.size(); i++)
			{
				pData->Contacts.push_back(pair<AglVector3, AglVector3>(points[i] + Normal * Penetration / 2, points[i] - Normal * Penetration / 2));
			}
			pData->Body1 = pR1;
			pData->Body2 = pR2;
		}
		else if (apoints.size() == 2 && bpoints.size() == 2)//Case: Edge-Edge
		{
			//Do not ignore! This is vital Do a distance check between edges.
			float s1 = 0, s2 = 0;
			EdgeEdgeDistance(apoints[0], apoints[1], bpoints[0], bpoints[1], s1, s2);
			pData->Body1 = pR1;
			pData->Body2 = pR2;
			pData->Contacts.push_back(pair<AglVector3, AglVector3>(apoints[0] + (apoints[1] - apoints[0]) * s1, bpoints[0] + (bpoints[1] - bpoints[0]) * s2));
		}
		else if (apoints.size() == 1 && bpoints.size() == 3)//Case: Face-Vertex
		{
			pData->Contacts.push_back(pair<AglVector3, AglVector3>(apoints[0], apoints[0] - Normal * Penetration));
			pData->Body1 = pR1;
			pData->Body2 = pR2;
		}
		else if (apoints.size() == 3 && bpoints.size() == 1)//Case: Face-Vertex
		{
			pData->Contacts.push_back(pair<AglVector3, AglVector3>(bpoints[0] + Normal * Penetration, bpoints[0]));
			pData->Body1 = pR1;
			pData->Body2 = pR2;
		}
		else
		{
			return false;
		}
		return true;
	}
	return false;
}
bool CheckCollision(RigidBodyBox* pB, RigidBodyConvexHull* pH, PhyCollisionData* pData)
{
	if (!CheckCollision(pH->GetBoundingSphere(), pB))
		return false;
	EPACollisionData EPAData;

	bool col = gjkCheckCollision(pH->GetExteriorPoints(), pB->GetCorners(), &EPAData);
	if (col)
	{
		AglVector3 Normal = EPAData.Normal;
		float Penetration = EPAData.Depth;

		if (AglVector3::dotProduct(pH->GetPosition() - pB->GetPosition(), Normal) < 0)
			Normal = -Normal;

		vector<AglVector3> apoints = GetHitPoints(pB, Normal, Penetration);
		vector<AglVector3> bpoints = GetHitPoints(pH->GetExteriorPoints(), -Normal, Penetration);

		//Vertex-Vertex, Vertex-Edge, Vertex-Face, Edge-Edge, Edge-Face, Face-Face

		if (apoints.size() == 4 && bpoints.size() == 3)//Case: Face - Face
		{
			vector<AglVector3> points = FindOverlapQuadTriangle(apoints, bpoints, Normal, Penetration);
			for (unsigned int i = 0; i < points.size(); i++)
			{
				pData->Contacts.push_back(pair<AglVector3, AglVector3>(points[i] + Normal * Penetration / 2, points[i] - Normal * Penetration / 2));
			}
			pData->Body1 = pB;
			pData->Body2 = pH;
		}
		else if (apoints.size() == 4 && bpoints.size() == 2)//Case: Edge-Face
		{
			vector<AglVector3> points = FindOverlapEdgeQuad(bpoints[0], bpoints[1], apoints, -Normal, Penetration);
			for (unsigned int i = 0; i < points.size(); i++)
			{
				pData->Contacts.push_back(pair<AglVector3, AglVector3>(points[i] + Normal * Penetration / 2, points[i] - Normal * Penetration / 2));
			}
			pData->Body1 = pB;
			pData->Body2 = pH;
		}
		else if (apoints.size() == 2 && bpoints.size() == 3)//Case: Edge-Face
		{
			vector<AglVector3> points = FindOverlapEdgeTriangle(apoints[0], apoints[1], bpoints, Normal, Penetration);
			for (unsigned int i = 0; i < points.size(); i++)
			{
				pData->Contacts.push_back(pair<AglVector3, AglVector3>(points[i] + Normal * Penetration / 2, points[i] - Normal * Penetration / 2));
			}
			pData->Body1 = pB;
			pData->Body2 = pH;
		}
		else if (apoints.size() == 2 && bpoints.size() == 2)//Case: Edge-Edge
		{
			//Do not ignore! This is vital Do a distance check between edges.
			float s1 = 0, s2 = 0;
			EdgeEdgeDistance(apoints[0], apoints[1], bpoints[0], bpoints[1], s1, s2);
			pData->Body1 = pB;
			pData->Body2 = pH;
			pData->Contacts.push_back(pair<AglVector3, AglVector3>(apoints[0] + (apoints[1] - apoints[0]) * s1, bpoints[0] + (bpoints[1] - bpoints[0]) * s2));
		}
		else if (apoints.size() == 1 && bpoints.size() == 3)//Case: Face-Vertex
		{
			pData->Contacts.push_back(pair<AglVector3, AglVector3>(apoints[0], apoints[0] - Normal * Penetration));
			pData->Body1 = pB;
			pData->Body2 = pH;
		}
		else if (apoints.size() == 4 && bpoints.size() == 1)//Case: Face-Vertex
		{
			pData->Contacts.push_back(pair<AglVector3, AglVector3>(bpoints[0] + Normal * Penetration, bpoints[0]));
			pData->Body1 = pB;
			pData->Body2 = pH;
		}
		else
		{
			return false;
		}
		return true;
	}
	return false;
}

bool CheckCollision(RigidBodySphere* p_sphere, RigidBodyMesh* p_mesh, 
					PhyCollisionData* p_collisionData)
{
	if (CheckCollision(p_sphere->GetBoundingSphere(), p_mesh->GetOBB()))
	{
		vector<EPACollisionData> epaCol;
		if (p_mesh->EvaluateSphere(p_sphere, epaCol))
		{
			p_collisionData->Body2 = p_sphere;
			p_collisionData->Body1 = p_mesh;
			for (unsigned int i = 0; i < epaCol.size(); i++)
			{
				pair<AglVector3, AglVector3> contact;
				AglVector3 dir = epaCol[i].Normal;
				AglVector3::normalize(dir);
				contact.second = p_sphere->GetPosition() - dir * p_sphere->GetRadius();
				contact.first = contact.second + dir * epaCol[i].Depth;
				p_collisionData->Contacts.push_back(contact);
			}
			return true;
		}
	}
	return false;
}

bool CheckCollision(RigidBodyBox* p_box, RigidBodyMesh* p_mesh, 
					PhyCollisionData* p_collisionData)
{
	PhyCollisionData* pData = p_collisionData;
	if (!CheckCollision(p_mesh->GetBoundingSphere(), p_box))
		return false;
	if (!CheckCollision(p_box->GetBoundingSphere(), p_mesh->GetOBB()))
		return false;

	bool doesCollide = false;
	vector<AglVector3> triangles;
	vector<AglVector3> triangle(3);
	if (p_mesh->EvaluateBox(p_box, triangles))
	{
		pData->Body1 = p_box;
		pData->Body2 = p_mesh;

		AglMatrix meshWorld = p_mesh->GetWorld();
		for (unsigned int i = 0; i < triangles.size(); i+=3)
		{
			triangle[0] = triangles[i];
			triangle[1] = triangles[i+1];
			triangle[2] = triangles[i+2];
			triangle[0].transform(meshWorld);
			triangle[1].transform(meshWorld);
			triangle[2].transform(meshWorld);

			EPACollisionData EPAData;
			bool col = gjkCheckCollision(triangle, p_box->GetCorners(), &EPAData);
			if (col)
			{
				doesCollide = true;
				AglVector3 Normal = EPAData.Normal;
				float Penetration = EPAData.Depth;

				if (AglVector3::dotProduct(triangle[0] - p_box->GetPosition(), Normal) < 0)
					Normal = -Normal;

				vector<AglVector3> apoints = GetHitPoints(p_box, Normal, Penetration);
				vector<AglVector3> bpoints = GetHitPoints(triangle, -Normal, Penetration);

				//Vertex-Vertex, Vertex-Edge, Vertex-Face, Edge-Edge, Edge-Face, Face-Face

				if (apoints.size() == 4 && bpoints.size() == 3)//Case: Face - Face
				{
					vector<AglVector3> points = FindOverlapQuadTriangle(apoints, bpoints, Normal, Penetration);
					for (unsigned int i = 0; i < points.size(); i++)
					{
						pData->Contacts.push_back(pair<AglVector3, AglVector3>(points[i] + Normal * Penetration / 2, points[i] - Normal * Penetration / 2));
					}
				}
				else if (apoints.size() == 4 && bpoints.size() == 2)//Case: Edge-Face
				{
					vector<AglVector3> points = FindOverlapEdgeQuad(bpoints[0], bpoints[1], apoints, -Normal, Penetration);
					for (unsigned int i = 0; i < points.size(); i++)
					{
						pData->Contacts.push_back(pair<AglVector3, AglVector3>(points[i] + Normal * Penetration / 2, points[i] - Normal * Penetration / 2));
					}
				}
				else if (apoints.size() == 2 && bpoints.size() == 3)//Case: Edge-Face
				{
					vector<AglVector3> points = FindOverlapEdgeTriangle(apoints[0], apoints[1], bpoints, Normal, Penetration);
					for (unsigned int i = 0; i < points.size(); i++)
					{
						pData->Contacts.push_back(pair<AglVector3, AglVector3>(points[i] + Normal * Penetration / 2, points[i] - Normal * Penetration / 2));
					}
				}
				else if (apoints.size() == 2 && bpoints.size() == 2)//Case: Edge-Edge
				{
					//Do not ignore! This is vital Do a distance check between edges.
					float s1 = 0, s2 = 0;
					EdgeEdgeDistance(apoints[0], apoints[1], bpoints[0], bpoints[1], s1, s2);
					pData->Contacts.push_back(pair<AglVector3, AglVector3>(apoints[0] + (apoints[1] - apoints[0]) * s1, bpoints[0] + (bpoints[1] - bpoints[0]) * s2));
				}
				else if (apoints.size() == 1 && bpoints.size() == 3)//Case: Face-Vertex
				{
					pData->Contacts.push_back(pair<AglVector3, AglVector3>(apoints[0], apoints[0] - Normal * Penetration));
				}
				else if (apoints.size() == 4 && bpoints.size() == 1)//Case: Face-Vertex
				{
					pData->Contacts.push_back(pair<AglVector3, AglVector3>(bpoints[0] + Normal * Penetration, bpoints[0]));
				}
				else
				{
				}
			}
		}
	}
	return doesCollide;
}

bool CheckCollision(RigidBodyConvexHull* p_hull, RigidBodyMesh* p_mesh, 
					PhyCollisionData* p_collisionData)
{
	return false;
}

bool CheckCollision(RigidBodyMesh* p_mesh1, RigidBodyMesh* p_mesh2, 
					PhyCollisionData* p_collisionData)
{
	bool doesCollide = false;
	PhyCollisionData* pData = p_collisionData;
	vector<AglVector3> triangles;
	
	if (p_mesh1->EvaluateMesh(p_mesh2, triangles))
	{
		pData->Body1 = p_mesh1;
		pData->Body2 = p_mesh2;
		vector<AglVector3> triangle1(3);
		vector<AglVector3> triangle2(3);
		AglMatrix mesh1World = p_mesh1->GetWorld();
		AglMatrix mesh2World = p_mesh2->GetWorld();
		for (unsigned int i = 0; i < triangles.size(); i+=6)
		{
			triangle1[0] = triangles[i];
			triangle1[1] = triangles[i+1];
			triangle1[2] = triangles[i+2];
			triangle1[0].transform(mesh1World);
			triangle1[1].transform(mesh1World);
			triangle1[2].transform(mesh1World);

			triangle2[0] = triangles[i+3];
			triangle2[1] = triangles[i+4];
			triangle2[2] = triangles[i+5];
			triangle2[0].transform(mesh2World);
			triangle2[1].transform(mesh2World);
			triangle2[2].transform(mesh2World);

			EPACollisionData EPAData;
			bool col = CheckCollision(triangle1[0], triangle1[1], triangle1[2],
									  triangle2[0], triangle2[1], triangle2[2],
									  &EPAData);//gjkCheckCollision(triangle1, triangle2, &EPAData);
			if (col)
			{
				doesCollide = true;
				AglVector3 Normal = EPAData.Normal;
				float Penetration = EPAData.Depth;

				AglVector3 c1 = (triangle1[0] + triangle1[1] + triangle1[2]) / 3;
				AglVector3 c2 = (triangle2[0] + triangle2[1] + triangle2[2]) / 3;
				if (AglVector3::dotProduct(c1 - c2, Normal) > 0)
					Normal = -Normal;

				vector<AglVector3> apoints = GetHitPoints(triangle2, Normal, Penetration);
				vector<AglVector3> bpoints = GetHitPoints(triangle1, -Normal, Penetration);

				//Vertex-Vertex, Vertex-Edge, Vertex-Face, Edge-Edge, Edge-Face, Face-Face

				if (apoints.size() == 3 && bpoints.size() == 3)//Case: Face - Face (Convex hulls are triangle based as opposed to boxes)
				{
					vector<AglVector3> points = FindOverlapTriangles(apoints, bpoints, Normal, Penetration);
					for (unsigned int i = 0; i < points.size(); i++)
					{
						pData->Contacts.push_back(pair<AglVector3, AglVector3>(points[i] + Normal * Penetration / 2, points[i] - Normal * Penetration / 2));
					}
				}
				else if (apoints.size() == 3 && bpoints.size() == 2)//Case: Edge-Face
				{
					vector<AglVector3> points = FindOverlapEdgeTriangle(bpoints[0], bpoints[1], apoints, -Normal, Penetration);
					for (unsigned int i = 0; i < points.size(); i++)
					{
						pData->Contacts.push_back(pair<AglVector3, AglVector3>(points[i] + Normal * Penetration / 2, points[i] - Normal * Penetration / 2));
					}
				}
				else if (apoints.size() == 2 && bpoints.size() == 3)//Case: Edge-Face
				{
					vector<AglVector3> points = FindOverlapEdgeTriangle(apoints[0], apoints[1], bpoints, Normal, Penetration);
					for (unsigned int i = 0; i < points.size(); i++)
					{
						pData->Contacts.push_back(pair<AglVector3, AglVector3>(points[i] + Normal * Penetration / 2, points[i] - Normal * Penetration / 2));
					}
				}
				else if (apoints.size() == 2 && bpoints.size() == 2)//Case: Edge-Edge
				{
					//Do not ignore! This is vital Do a distance check between edges.
					float s1 = 0, s2 = 0;
					EdgeEdgeDistance(apoints[0], apoints[1], bpoints[0], bpoints[1], s1, s2);
					pData->Contacts.push_back(pair<AglVector3, AglVector3>(apoints[0] + (apoints[1] - apoints[0]) * s1, bpoints[0] + (bpoints[1] - bpoints[0]) * s2));
				}
				else if (apoints.size() == 1 && bpoints.size() == 3)//Case: Face-Vertex
				{
					pData->Contacts.push_back(pair<AglVector3, AglVector3>(apoints[0], apoints[0] - Normal * Penetration));
				}
				else if (apoints.size() == 3 && bpoints.size() == 1)//Case: Face-Vertex
				{
					pData->Contacts.push_back(pair<AglVector3, AglVector3>(bpoints[0] + Normal * Penetration, bpoints[0]));
				}
				else
				{
				}
			}
		}
	}
	return doesCollide;
}

bool CheckCollision(const AglBoundingSphere& p_sphere, const AglVector3& p_v1, const AglVector3& p_v2, const AglVector3& p_v3,
					EPACollisionData* p_epaData)
{
	//Sphere min = projected pos + radius
	//Sphere max = projected pos - radius

	AglVector3 p = p_sphere.position;
	AglVector3 axes[7];
	axes[0] = AglVector3::crossProduct(p_v2-p_v1, p_v3-p_v2);
	axes[1] = p_v1 - p;
	axes[2] = p_v2 - p;
	axes[3] = p_v3 - p;
	axes[4] = AglVector3::crossProduct(AglVector3::crossProduct(p_v2-p_v1, p - p_v1), p_v2 - p_v1);
	axes[5] = AglVector3::crossProduct(AglVector3::crossProduct(p_v3-p_v1, p - p_v1), p_v3 - p_v1);
	axes[6] = AglVector3::crossProduct(AglVector3::crossProduct(p_v3-p_v2, p - p_v2), p_v3 - p_v2);

	float minA, maxA;
	float minB, maxB;

	//Project points on the axis
	float overlap = FLT_MAX;
	AglVector3 axis;
	for (unsigned int i = 0; i < 7; i++)
	{
		axes[i].normalize();
		minA = maxA = AglVector3::dotProduct(axes[i], p_v1);
		minA = min(AglVector3::dotProduct(axes[i], p_v2), minA);
		minA = min(AglVector3::dotProduct(axes[i], p_v3), minA);
		maxA = max(AglVector3::dotProduct(axes[i], p_v2), maxA);
		maxA = max(AglVector3::dotProduct(axes[i], p_v3), maxA);

		minB = AglVector3::dotProduct(axes[i], p) - p_sphere.radius;
		maxB = AglVector3::dotProduct(axes[i], p) + p_sphere.radius;
		
		if (i > 0)
		{
			float lengthA = maxA - minA;
			float lengthB = maxB - minB;

			float minTotal = min(minA, minB);
			float maxTotal = max(maxA, maxB);

			float newOverlap = (lengthA + lengthB) - (maxTotal - minTotal);
			if (newOverlap <= 0)
			{
				return false;
			}
			else if (newOverlap < overlap)
			{
				overlap = newOverlap;
				axis = axes[i];
			}
		}
		else
		{
			float newOverlap = min(minA - minB, maxB - minA);
			if (newOverlap < 0)
				return false;
			overlap = newOverlap;
			axis = axes[i];
		}
	}

	p_epaData->Depth = overlap;
	p_epaData->Normal = axis;
	return true;
}

bool CheckCollision(const AglVector3& p_t11, const AglVector3& p_t12, const AglVector3& p_t13,
					const AglVector3& p_t21, const AglVector3& p_t22, const AglVector3& p_t23,
					EPACollisionData* p_epaData)
{
	AglVector3 axes[11];
	axes[0] = AglVector3::crossProduct(p_t12-p_t11, p_t13-p_t11); //Normal
	axes[1] = AglVector3::crossProduct(p_t22-p_t21, p_t23-p_t21); //Normal
	axes[2] = AglVector3::crossProduct(p_t12-p_t11, p_t22 - p_t21);
	axes[3] = AglVector3::crossProduct(p_t12-p_t11, p_t23 - p_t21);
	axes[4] = AglVector3::crossProduct(p_t12-p_t11, p_t23 - p_t22);
	axes[5] = AglVector3::crossProduct(p_t13-p_t11, p_t22 - p_t21);
	axes[6] = AglVector3::crossProduct(p_t13-p_t11, p_t23 - p_t21);
	axes[7] = AglVector3::crossProduct(p_t13-p_t11, p_t23 - p_t22);
	axes[8] = AglVector3::crossProduct(p_t13-p_t12, p_t22 - p_t21);
	axes[9] = AglVector3::crossProduct(p_t13-p_t12, p_t23 - p_t21);
	axes[10] = AglVector3::crossProduct(p_t13-p_t12, p_t23 - p_t22);

	float minA, maxA;
	float minB, maxB;

	//Project points on the axis
	float overlap = FLT_MAX;
	AglVector3 axis;
	for (unsigned int i = 0; i < 11; i++)
	{
		// min/max a
		axes[i].normalize();
		minA = maxA = AglVector3::dotProduct(axes[i], p_t11);
		minA = min(AglVector3::dotProduct(axes[i], p_t12), minA);
		minA = min(AglVector3::dotProduct(axes[i], p_t13), minA);
		maxA = max(AglVector3::dotProduct(axes[i], p_t12), maxA);
		maxA = max(AglVector3::dotProduct(axes[i], p_t13), maxA);

		// min/max b
		minB = maxB = AglVector3::dotProduct(axes[i], p_t21);
		minB = min(AglVector3::dotProduct(axes[i], p_t22), minB);
		minB = min(AglVector3::dotProduct(axes[i], p_t23), minB);
		maxB = max(AglVector3::dotProduct(axes[i], p_t22), maxB);
		maxB = max(AglVector3::dotProduct(axes[i], p_t23), maxB);

		if (i > 1)
		{
			float lengthA = maxA - minA;
			float lengthB = maxB - minB;

			float minTotal = min(minA, minB);
			float maxTotal = max(maxA, maxB);

			float newOverlap = (lengthA + lengthB) - (maxTotal - minTotal);
			if (newOverlap <= 0)
			{
				return false;
			}
			else if (newOverlap < overlap)
			{
				overlap = newOverlap;
				axis = axes[i];
			}
		}
		else if (i == 0)
		{
			float newOverlap = min(minA - minB, maxB - minA);
			if (newOverlap < 0)
				return false;
			overlap = newOverlap;
			axis = axes[i];
		}
		else
		{
			float newOverlap = min(minB - minA, maxA - minB);
			if (newOverlap < 0)
				return false;
			overlap = newOverlap;
			axis = axes[i];
		}
	}

	p_epaData->Depth = overlap;
	p_epaData->Normal = axis;
	return true;
}

//---------------------------------SUPPORT FUNCTIONS--------------------------------------
float OverlapAmount(RigidBodyBox* pB1, RigidBodyBox* pB2, AglVector3 pAxis)
{
	AglVector3::normalize(pAxis);

    float minA = 0, maxA = 0, minB = 0, maxB = 0;
    CalculateProjectionInterval(pB1, pAxis, minA, maxA);
    CalculateProjectionInterval(pB2, pAxis, minB, maxB);

    float lengthA = maxA - minA;
    float lengthB = maxB - minB;

    float minTotal = min(minA, minB);
    float maxTotal = max(maxA, maxB);

    if (maxTotal - minTotal >= lengthA + lengthB)
    {
        return 0;
    }
    return lengthA + lengthB - (maxTotal - minTotal);
}
void  CalculateProjectionInterval(RigidBodyBox* pBox, AglVector3 pAxis, float& pMin, float& pMax)
{
	vector<AglVector3> corn = pBox->GetCorners();

    pMin = pMax = AglVector3::dotProduct(pAxis, corn[0]);
    for (int i = 1; i < 8; i++)
    {
        float curr = AglVector3::dotProduct(pAxis, corn[i]);
        if (curr < pMin)
            pMin = curr;
        else if (curr > pMax)
            pMax = curr;
    }
}

float OverlapAmount(const vector<AglVector3>& p_points1, const vector<AglVector3>& p_points2, const AglVector3& p_axis)
{
	//Assumed to be normalized
	//AglVector3::normalize(pAxis);

	float minA = 0, maxA = 0, minB = 0, maxB = 0;
	CalculateProjectionInterval(p_points1, p_axis, minA, maxA);
	CalculateProjectionInterval(p_points2, p_axis, minB, maxB);

	float lengthA = maxA - minA;
	float lengthB = maxB - minB;

	float minTotal = min(minA, minB);
	float maxTotal = max(maxA, maxB);

	if (maxTotal - minTotal >= lengthA + lengthB)
	{
		return 0;
	}
	return lengthA + lengthB - (maxTotal - minTotal);
}

void  CalculateProjectionInterval(const vector<AglVector3>& p_points, const AglVector3& p_axis, 
								  float& p_min, float& p_max)
{
	p_min = p_max = AglVector3::dotProduct(p_axis, p_points[0]);
	for (int i = 1; i < 8; i++)
	{
		float curr = AglVector3::dotProduct(p_axis, p_points[i]);
		if (curr < p_min)
			p_min = curr;
		else if (curr > p_max)
			p_max = curr;
	}
}

//NEW - OPTIMIZED

float OverlapAmount(const AglVector3* p_points1, const AglVector3* p_points2, const AglVector3& p_axis)
{
	//Assumed to be normalized
	//AglVector3::normalize(pAxis);

	float minA = 0, maxA = 0, minB = 0, maxB = 0;
	CalculateProjectionInterval(p_points1, p_axis, minA, maxA);
	CalculateProjectionInterval(p_points2, p_axis, minB, maxB);

	float lengthA = maxA - minA;
	float lengthB = maxB - minB;

	float minTotal = min(minA, minB);
	float maxTotal = max(maxA, maxB);

	if (maxTotal - minTotal >= lengthA + lengthB)
	{
		return 0;
	}
	return lengthA + lengthB - (maxTotal - minTotal);
}

void  CalculateProjectionInterval(const AglVector3* p_points, const AglVector3& p_axis, 
								  float& p_min, float& p_max)
{
	p_min = p_max = AglVector3::dotProduct(p_axis, p_points[0]);
	for (int i = 1; i < 8; i++)
	{
		float curr = p_axis.x * p_points[i].x + p_axis.y * p_points[i].y + p_axis.z * p_points[i].z;
		if (curr < p_min)
			p_min = curr;
		else if (curr > p_max)
			p_max = curr;
	}
}

//NEW -- OPTIMIZED END


vector<AglVector3> GetHitPoints(RigidBodyBox* pBox, AglVector3 pNormal, float pPenetration)
{
	//Given a collision normal and a penetration amount; find the collision points
    //of a given OBB

    //Get the corners of the OBB in world space
    vector<AglVector3> corners = pBox->GetCorners();

    //Find the point of maximum overlap
    AglVector3 maxPoint = corners[0];
    float maxPhyPlaneDist = AglVector3::dotProduct(corners[0], pNormal);
    for (unsigned int i = 1; i < corners.size(); i++)
    {
        float dist = AglVector3::dotProduct(corners[i], pNormal);
        if (dist > maxPhyPlaneDist)
        {
            maxPhyPlaneDist = dist;
            maxPoint = corners[i];
        }
    }

    //Use this point to derive a PhyPlane separating the overlapping
    //points from the non-overlapping
    float d = maxPhyPlaneDist;
    d -= pPenetration + 0.0001f;

    //Find the overlapping points by checking each point against the
    //PhyPlane.
    vector<AglVector3> hitPoints;
    for (unsigned int i = 0; i < corners.size(); i++)
    {
        float side = AglVector3::dotProduct(corners[i], pNormal) - d;
        if (side > 0)
		{
            hitPoints.push_back(corners[i]);
		}
    }

    //Haxx - must generalize this so it works for all configurations. It seems to have solved the problem though! :D
	if (hitPoints.size() == 3)
    {
        float max = AglVector3::dotProduct(hitPoints[0], pNormal) - d;
        int ind = 0;
        for (unsigned int i = 1; i < hitPoints.size(); i++)
        {
            float side = AglVector3::dotProduct(hitPoints[i], pNormal) - d;
            if (side > max)
            {
                max = side;
                ind = i;
            }
        }
        AglVector3 p = hitPoints[ind];
		hitPoints.clear();
        hitPoints.push_back(p);
    }

    if (hitPoints.size() > 4)
    {
        float max = AglVector3::dotProduct(hitPoints[0], pNormal) - d;
        int ind = 0;
        for (unsigned int i = 1; i < hitPoints.size(); i++)
        {
            float side = AglVector3::dotProduct(hitPoints[i], pNormal) - d;
            if (side > max)
            {
                max = side;
                ind = i;
            }
        }

        vector<AglVector3> newPoints;
        for (unsigned int i = 0; i < hitPoints.size(); i++)
        {
            float side = AglVector3::dotProduct(hitPoints[i], pNormal) - d;
            if (side > max - 0.001f)
				newPoints.push_back(hitPoints[i]);
        }
        hitPoints = newPoints;
    }
    return hitPoints;
}
vector<AglVector3> GetHitPoints(vector<AglVector3> pPoints, AglVector3 pNormal, float pPenetration)
{
    //Find the point of maximum overlap
    AglVector3 maxPoint = pPoints[0];
    float maxPhyPlaneDist = AglVector3::dotProduct(pPoints[0], pNormal);
    for (unsigned int i = 1; i < pPoints.size(); i++)
    {
        float dist = AglVector3::dotProduct(pPoints[i], pNormal);
        if (dist > maxPhyPlaneDist)
        {
            maxPhyPlaneDist = dist;
            maxPoint = pPoints[i];
        }
    }

    //Use this point to derive a PhyPlane separating the overlapping
    //points from the non-overlapping
    float d = maxPhyPlaneDist;
    d -= pPenetration + 0.0001f;

    //Find the overlapping points by checking each point against the
    //PhyPlane.
    vector<AglVector3> hitPoints;
    for (unsigned int i = 0; i < pPoints.size(); i++)
    {
        float side = AglVector3::dotProduct(pPoints[i], pNormal) - d;
        if (side > 0)
            hitPoints.push_back(pPoints[i]);
    }

	//Takes care of extreme cases. Haxx Solution for Convex Hulls
	//Consult Anton for more information.
	if (hitPoints.size() > 3)
	{
		AglVector3 p1 = hitPoints[0];
		AglVector3 p2 = hitPoints[1];
		AglVector3 p3 = hitPoints[2];
		hitPoints.clear();
		hitPoints.push_back(p1);
		hitPoints.push_back(p2);
		hitPoints.push_back(p3);
	}

    return hitPoints;
}
vector<AglVector3> FindOverlapQuads(vector<AglVector3> pApoints, vector<AglVector3> pBpoints, AglVector3 pAxis, float pPenetration)
{
	//Project all of the points on a common PhyPlane right between the two colliding obb
    for (int i = 0; i < 4; i++)
    {
        pApoints[i] -= pAxis * pPenetration * 0.5f;
        pBpoints[i] += pAxis * pPenetration * 0.5f;
    }


    pApoints = SortQuad(pApoints);
    pBpoints = SortQuad(pBpoints);

    vector<PhyPlane> PhyPlaneA;
    vector<PhyPlane> PhyPlaneB;

    //Create PhyPlanes for the first quad
    AglVector3 c1 = AglVector3(0, 0, 0);

    for (int i = 0; i < 4; i++)
        c1 += pApoints[i];
    c1 /= 4;

    for (int i = 0; i < 4; i++)
    {
        AglVector3 t = pApoints[(i + 1) % 4] - pApoints[i % 4];
        AglVector3 n = AglVector3::crossProduct(pAxis, t);
        AglVector3::normalize(n);
        if (AglVector3::dotProduct(n, pApoints[i % 4] - c1) < 0)
            n = -n;
        float d = -AglVector3::dotProduct(pApoints[i % 4], n); 
		PhyPlaneA.push_back(PhyPlane(n, d));
    }

    //Create PhyPlanes for the second quad
    AglVector3 c2 = AglVector3(0, 0, 0);
    for (int i = 0; i < 4; i++)
        c2 += pBpoints[i];
    c2 /= 4;

    for (int i = 0; i < 4; i++)
    {
        AglVector3 t = pBpoints[(i + 1) % 4] - pBpoints[i % 4];
        AglVector3 n = AglVector3::crossProduct(pAxis, t);
        AglVector3::normalize(n);
        if (AglVector3::dotProduct(n, pBpoints[i % 4] - c2) < 0)
            n = -n;
        float d = -AglVector3::dotProduct(pBpoints[i % 4], n);
		PhyPlaneB.push_back(PhyPlane(n, d));
    }

    vector<AglVector3> final;

    for (unsigned int i = 0; i < pApoints.size(); i++)
    {
        bool inside = true;
        for (unsigned int j = 0; j < PhyPlaneB.size(); j++)
        {
            if (AglVector3::dotProduct(pApoints[i], PhyPlaneB[j].N) + PhyPlaneB[j].D > 0.0001f)
                inside = false;
        }
        if (inside)
            final.push_back(pApoints[i]);
    }

    if (final.size() == 4) //|| final.size() == 3 ????
        return final;

    for (unsigned int i = 0; i < pBpoints.size(); i++)
    {
        bool inside = true;
        for (unsigned int j = 0; j < PhyPlaneA.size(); j++)
        {
            if (AglVector3::dotProduct(pBpoints[i], PhyPlaneA[j].N) + PhyPlaneA[j].D > 0.0001f)
                inside = false;
        }
        if (inside)
            final.push_back(pBpoints[i]);
    }

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            float s = 0, t = 0;
            float dist = EdgeEdgeDistance(pApoints[(i + 1) % 4], pApoints[i % 4], pBpoints[(j + 1) % 4], pBpoints[j % 4], s, t);
            if (dist < 0.0002f)
            {
                if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
                    final.push_back(pApoints[(i+1) % 4] + (pApoints[i % 4] - pApoints[(i+1) % 4]) * s);
            }
        }
    }

    return final;
}
vector<AglVector3> FindOverlapQuadTriangle(vector<AglVector3> pQuad, vector<AglVector3> pTriangle, AglVector3 pAxis, float pPenetration)
{
	//Project all of the points on a common PhyPlane right between the two colliding obb
    for (int i = 0; i < 4; i++)
    {
        pQuad[i] -= pAxis * pPenetration * 0.5f;
    }
	for (int i = 0; i < 3; i++)
	{
		pTriangle[i] += pAxis * pPenetration * 0.5f;
	}


    pQuad = SortQuad(pQuad);

    vector<PhyPlane> PhyPlaneA;
    vector<PhyPlane> PhyPlaneB;

    //Create PhyPlanes for the quad
    AglVector3 c1 = AglVector3(0, 0, 0);
    for (int i = 0; i < 4; i++)
        c1 += pQuad[i];
    c1 /= 4;

    for (int i = 0; i < 4; i++)
    {
        AglVector3 t = pQuad[(i + 1) % 4] - pQuad[i % 4];
        AglVector3 n = AglVector3::crossProduct(pAxis, t);
        AglVector3::normalize(n);
        if (AglVector3::dotProduct(n, pQuad[i % 4] - c1) < 0)
            n = -n;
        float d = -AglVector3::dotProduct(pQuad[i % 4], n); 
		PhyPlaneA.push_back(PhyPlane(n, d));
    }

    //Create PhyPlanes for the triangle
    AglVector3 c2 = AglVector3(0, 0, 0);
    for (int i = 0; i < 3; i++)
        c2 += pTriangle[i];
    c2 /= 3;

    for (int i = 0; i < 3; i++)
    {
        AglVector3 t = pTriangle[(i + 1) % 3] - pTriangle[i % 3];
        AglVector3 n = AglVector3::crossProduct(pAxis, t);
        AglVector3::normalize(n);
        if (AglVector3::dotProduct(n, pTriangle[i % 3] - c2) < 0)
            n = -n;
        float d = -AglVector3::dotProduct(pTriangle[i % 3], n);
		PhyPlaneB.push_back(PhyPlane(n, d));
    }

    vector<AglVector3> final;

    for (unsigned int i = 0; i < pQuad.size(); i++)
    {
        bool inside = true;
        for (unsigned int j = 0; j < PhyPlaneB.size(); j++)
        {
            if (AglVector3::dotProduct(pQuad[i], PhyPlaneB[j].N) + PhyPlaneB[j].D > 0.0001f)
                inside = false;
        }
        if (inside)
            final.push_back(pQuad[i]);
    }

    if (final.size() == 4)
        return final;

    for (unsigned int i = 0; i < pTriangle.size(); i++)
    {
        bool inside = true;
        for (unsigned int j = 0; j < PhyPlaneA.size(); j++)
        {
            if (AglVector3::dotProduct(pTriangle[i], PhyPlaneA[j].N) + PhyPlaneA[j].D > 0.0001f)
                inside = false;
        }
        if (inside)
            final.push_back(pTriangle[i]);
    }

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            float s = 0, t = 0;
            float dist = EdgeEdgeDistance(pQuad[(i + 1) % 4], pQuad[i % 4], pTriangle[(j + 1) % 3], pTriangle[j % 3], s, t);
            if (dist < 0.0002f)
            {
                if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
                    final.push_back(pQuad[(i+1) % 4] + (pQuad[i % 4] - pQuad[(i+1) % 4]) * s);
            }
        }
    }

    return final;
}
vector<AglVector3> FindOverlapTriangles(vector<AglVector3> pApoints, vector<AglVector3> pBpoints, AglVector3 pAxis, float pPenetration)
{
	//Project all of the points on a common PhyPlane right between the two colliding obb
    for (int i = 0; i < 3; i++)
    {
        pApoints[i] -= pAxis * pPenetration * 0.5f;
        pBpoints[i] += pAxis * pPenetration * 0.5f;
    }

    vector<PhyPlane> PhyPlaneA;
    vector<PhyPlane> PhyPlaneB;

    //Create PhyPlanes for the first triangle
    AglVector3 c1 = AglVector3(0, 0, 0);
    for (int i = 0; i < 3; i++)
        c1 += pApoints[i];
    c1 /= 3;

    for (int i = 0; i < 3; i++)
    {
        AglVector3 t = pApoints[(i + 1) % 3] - pApoints[i % 3];
        AglVector3 n = AglVector3::crossProduct(pAxis, t);
        AglVector3::normalize(n);
        if (AglVector3::dotProduct(n, pApoints[i % 3] - c1) < 0)
            n = -n;
        float d = -AglVector3::dotProduct(pApoints[i % 3], n); 
		PhyPlaneA.push_back(PhyPlane(n, d));
    }

    //Create PhyPlanes for the second triangle
    AglVector3 c2 = AglVector3(0, 0, 0);
    for (int i = 0; i < 3; i++)
        c2 += pBpoints[i];
    c2 /= 3;

    for (int i = 0; i < 3; i++)
    {
        AglVector3 t = pBpoints[(i + 1) % 3] - pBpoints[i % 3];
        AglVector3 n = AglVector3::crossProduct(pAxis, t);
        AglVector3::normalize(n);
        if (AglVector3::dotProduct(n, pBpoints[i % 3] - c2) < 0)
            n = -n;
        float d = -AglVector3::dotProduct(pBpoints[i % 3], n);
		PhyPlaneB.push_back(PhyPlane(n, d));
    }

    vector<AglVector3> final;

    for (unsigned int i = 0; i < pApoints.size(); i++)
    {
        bool inside = true;
        for (unsigned int j = 0; j < PhyPlaneB.size(); j++)
        {
            if (AglVector3::dotProduct(pApoints[i], PhyPlaneB[j].N) + PhyPlaneB[j].D > 0.0001f)
                inside = false;
        }
        if (inside)
            final.push_back(pApoints[i]);
    }

    if (final.size() == 3)
        return final;

    for (unsigned int i = 0; i < pBpoints.size(); i++)
    {
        bool inside = true;
        for (unsigned int j = 0; j < PhyPlaneA.size(); j++)
        {
            if (AglVector3::dotProduct(pBpoints[i], PhyPlaneA[j].N) + PhyPlaneA[j].D > 0.0001f)
                inside = false;
        }
        if (inside)
            final.push_back(pBpoints[i]);
    }

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            float s = 0, t = 0;
            float dist = EdgeEdgeDistance(pApoints[(i + 1) % 3], pApoints[i % 3], pBpoints[(j + 1) % 3], pBpoints[j % 3], s, t);
            if (dist < 0.0002f)
            {
                if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
                    final.push_back(pApoints[(i+1) % 3] + (pApoints[i % 3] - pApoints[(i+1) % 3]) * s);
            }
        }
    }

    return final;
}
vector<AglVector3> FindOverlapEdgeQuad(AglVector3 pEdgeStart, AglVector3 pEdgeEnd, vector<AglVector3> pPoints, AglVector3 pAxis, float pPenetration)
{
    for (int i = 0; i < 4; i++)
    {
        pPoints[i] += pAxis * pPenetration * 0.5f;
    }
    pEdgeStart -= pAxis * pPenetration * 0.5f;
    pEdgeEnd -= pAxis * pPenetration * 0.5f;

    pPoints = SortQuad(pPoints);

    vector<PhyPlane> plane;

    //Create PhyPlanes for the first quad
    AglVector3 c = AglVector3(0, 0, 0);
    for (int i = 0; i < 4; i++)
        c += pPoints[i];
    c /= 4;

    for (int i = 0; i < 4; i++)
    {
        AglVector3 t = pPoints[(i + 1) % 4] - pPoints[i % 4];
        AglVector3 n = AglVector3::crossProduct(pAxis, t);
        AglVector3::normalize(n);
        if (AglVector3::dotProduct(n, pPoints[i % 4] - c) < 0)
            n = -n;
        float d = -AglVector3::dotProduct(pPoints[i % 4], n);
        plane.push_back(PhyPlane(n, d));
    }
    vector<AglVector3> final;

    bool inside = true;
    bool inside2 = true;
    for (unsigned int j = 0; j < plane.size(); j++)
    {
        if (AglVector3::dotProduct(pEdgeStart, plane[j].N) + plane[j].D > 0.0001f)
            inside = false;
        if (AglVector3::dotProduct(pEdgeEnd, plane[j].N) + plane[j].D > 0.0001f)
            inside2 = false;
    }
    if (inside)
        final.push_back(pEdgeStart);
    if (inside2)
        final.push_back(pEdgeEnd);

    if (final.size() == 2)
        return final;

    for (int i = 0; i < 4; i++)
    {
        float s = 0, t = 0;
        float dist = EdgeEdgeDistance(pPoints[(i + 1) % 4], pPoints[i % 4], pEdgeStart, pEdgeEnd, s, t);
        if (dist < 0.0002f)
        {
            if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
                final.push_back(pPoints[(i + 1) % 4] + (pPoints[i % 4] - pPoints[(i + 1) % 4]) * s);
        }
    }

    return final;
}
vector<AglVector3> FindOverlapEdgeTriangle(AglVector3 pEdgeStart, AglVector3 pEdgeEnd, vector<AglVector3> pPoints, AglVector3 pAxis, float pPenetration)
{
    for (int i = 0; i < 3; i++)
    {
        pPoints[i] += pAxis * pPenetration * 0.5f;
    }
    pEdgeStart -= pAxis * pPenetration * 0.5f;
    pEdgeEnd -= pAxis * pPenetration * 0.5f;

    vector<PhyPlane> plane;

    //Create PhyPlanes for the triangle
    AglVector3 c = AglVector3(0, 0, 0);
    for (int i = 0; i < 3; i++)
        c += pPoints[i];
    c /= 3;

    for (int i = 0; i < 3; i++)
    {
        AglVector3 t = pPoints[(i + 1) % 3] - pPoints[i % 3];
        AglVector3 n = AglVector3::crossProduct(pAxis, t);
        AglVector3::normalize(n);
        if (AglVector3::dotProduct(n, pPoints[i % 3] - c) < 0)
            n = -n;
        float d = -AglVector3::dotProduct(pPoints[i % 3], n);
        plane.push_back(PhyPlane(n, d));
    }
    vector<AglVector3> final;

    bool inside = true;
    bool inside2 = true;
    for (unsigned int j = 0; j < plane.size(); j++)
    {
        if (AglVector3::dotProduct(pEdgeStart, plane[j].N) + plane[j].D > 0.0001f)
            inside = false;
        if (AglVector3::dotProduct(pEdgeEnd, plane[j].N) + plane[j].D > 0.0001f)
            inside2 = false;
    }
    if (inside)
        final.push_back(pEdgeStart);
    if (inside2)
        final.push_back(pEdgeEnd);

    if (final.size() == 2)
        return final;

    for (int i = 0; i < 3; i++)
    {
        float s = 0, t = 0;
        float dist = EdgeEdgeDistance(pPoints[(i + 1) % 3], pPoints[i % 3], pEdgeStart, pEdgeEnd, s, t);
        if (dist < 0.0002f)
        {
            if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
                final.push_back(pPoints[(i + 1) % 3] + (pPoints[i % 3] - pPoints[(i + 1) % 3]) * s);
        }
    }

    return final;
}
vector<AglVector3> SortQuad(vector<AglVector3> pQuad)
{
	if (pQuad.size() != 4)
        return pQuad;

    if (AglVector3::lengthSquared(pQuad[1] - pQuad[0]) > AglVector3::lengthSquared(pQuad[2] - pQuad[0]))
    {
        AglVector3 t = pQuad[1];
        pQuad[1] = pQuad[2];
        pQuad[2] = t;
    }

    if (AglVector3::lengthSquared(pQuad[2] - pQuad[1]) > AglVector3::lengthSquared(pQuad[3] - pQuad[1]))
    {
        AglVector3 t = pQuad[2];
        pQuad[2] = pQuad[3];
        pQuad[3] = t;
    }
    return pQuad;
}
float EdgeEdgeDistance(AglVector3 pE1Start, AglVector3 pE1End, AglVector3 pE2Start, AglVector3 pE2End, float& pS, float& pT)
{
    //Assumes that the shortest distance is somewhere along the edges.
    //Does not hold true for lines in general.
    AglVector3 u = pE1End - pE1Start;
    AglVector3 v = pE2End - pE2Start;
    AglVector3 w = pE1Start - pE2Start;

    float a = AglVector3::dotProduct(u, u);
    float b = AglVector3::dotProduct(u, v);
    float c = AglVector3::dotProduct(v, v);
    float d = AglVector3::dotProduct(u, w);
    float e = AglVector3::dotProduct(v, w);

    pS = (b * e - c * d) / (a * c - b * b);
    pT = (a * e - b * d) / (a * c - b * b);
    return AglVector3::length((pE1Start + (pE1End - pE1Start) * pS) - (pE2Start + (pE2End - pE2Start) * pT));

}
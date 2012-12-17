#include "GJKSolver.h"
#include <list>
#include "RigidBody.h"

bool gjkCheckCollision(const vector<AglVector3>& pA, const vector<AglVector3>& pB, EPACollisionData* pData)
{
	vector<AglVector3> S;
	//Calculate the initial support point
	AglVector3 s = gjkSupport(pA[0] - pB[0], pA, pB);

	//Should be okay. This means that the objects are in touching contact. 
	//Therefore the "penetration" can be seen as exactly zero. 
	if (s == AglVector3(0, 0, 0))
		return false;

	//Push the support point on the support point list.
	//Ensure that the newest support point is always at
	//the end of the support list.
	GJKSIZE = 1;
	GJKLIST[0] = s;

	AglVector3 d = -s;
	bool done = false;

	while (!done)
	{
		s = gjkSupport(d, pA, pB);
		if (AglVector3::dotProduct(s, d) < 0.00001f)
			return false;

		GJKLIST[GJKSIZE++] = s;
		done = gjkProcessSimplex(d);
	}
	S.push_back(GJKLIST[0]);
	S.push_back(GJKLIST[1]);
	S.push_back(GJKLIST[2]);
	S.push_back(GJKLIST[3]);
	epaProcessCollision(S, pA, pB, pData);
	return true;
}
bool gjkCheckCollision(const vector<AglVector3>& pA, const AglBoundingSphere& pSphere, EPACollisionData* pData)
{
	vector<AglVector3> S;
	//Calculate the initial support point
	AglVector3 dir = pA[0] - pSphere.position;
	AglVector3 s = gjkSupport(dir, pA, pSphere);

	//Should be okay. This means that the objects are in touching contact. 
	//Therefore the "penetration" can be seen as exactly zero. 
	if (s == AglVector3(0, 0, 0))
		return false;

	//Push the support point on the support point list.
	//Ensure that the newest support point is always at
	//the end of the support list.
	GJKSIZE = 1;
	GJKLIST[0] = s;

	AglVector3 d = -s;
	bool done = false;

	while (!done)
	{
		s = gjkSupport(d, pA, pSphere);
		if (AglVector3::dotProduct(s, d) < 0.00001f)
			return false;

		GJKLIST[GJKSIZE++] = s;
		done = gjkProcessSimplex(d);
	}
	S.push_back(GJKLIST[0]);
	S.push_back(GJKLIST[1]);
	S.push_back(GJKLIST[2]);
	S.push_back(GJKLIST[3]);
	epaProcessCollision(S, pA, pSphere, pData);
	return true;
}

AglVector3 gjkSupport(AglVector3 pDirection, const vector<AglVector3>& pA, const vector<AglVector3>& pB)
{
	//Do not need to normalize. the > query will work
	//no matter the length of pDirection
	//AglVector3::normalize(pDirection);

	int ai = 0, bi = 0;

	float a = AglVector3::dotProduct(pA[ai], pDirection);

	unsigned int i;
	for (i = 1; i < pA.size(); i++)
	{
		float val = AglVector3::dotProduct(pA[i], pDirection);
		if (val > a)
		{
			ai = i;
			a = val;
		}
	}
	a = -a;
	for (i = 1; i < pB.size(); i++)
	{
		float val = AglVector3::dotProduct(pB[i], -pDirection);
		if (val > a)
		{
			bi = i;
			a = val;
		}
	}
	return pA[ai] - pB[bi];
}
AglVector3	gjkSupport(AglVector3 pDirection, const vector<AglVector3>& pA, const AglBoundingSphere& pSphere)
{
	//Need to normalize for sphere. I think
	AglVector3::normalize(pDirection);

	int ai = 0, bi = 0;

	float a = AglVector3::dotProduct(pA[ai], pDirection);

	unsigned int i;
	for (i = 1; i < pA.size(); i++)
	{
		float val = AglVector3::dotProduct(pA[i], pDirection);
		if (val > a)
		{
			ai = i;
			a = val;
		}
	}
	pDirection = -pDirection;
	AglVector3 spherePos = pSphere.position + pDirection * pSphere.radius; 
	return pA[ai] - spherePos;
}
bool gjkProcessSimplex(AglVector3& pDirection)
{
	if (GJKSIZE == 2)//pPointList.size() == 2)
	{
		return gjkProcessSimplexLine(pDirection);
	}
	else if (GJKSIZE == 3)//pPointList.size() == 3)
	{
		return gjkProcessSimplexTriangle(pDirection);
	}

	return gjkProcessSimplexTetra(pDirection);
}
bool gjkProcessSimplexLine(AglVector3& pDirection)
{
	AglVector3 A = GJKLIST[1];
	AglVector3 B = GJKLIST[0];

	AglVector3 cross = AglVector3::crossProduct(B-A, -A);
	if (AglVector3::dotProduct(B-A, -A) > 0)
	{
		pDirection = AglVector3::crossProduct(cross, B-A);
	}
	else
	{
		//Does it ever enter this?
		gjkSetPoint(A, pDirection);
	}
	return false;
}
bool gjkProcessSimplexTriangle(AglVector3& pDirection)
{
	AglVector3 A = GJKLIST[2];
	AglVector3 B = GJKLIST[1];
	AglVector3 C = GJKLIST[0];

	//Winding order here is importanat. If the algorithm fails check this.
	AglVector3 ABC = AglVector3::crossProduct(B-A, C-A);

	if (AglVector3::dotProduct(AglVector3::crossProduct(ABC, C-A), -A) > 0)
	{
		if (AglVector3::dotProduct(C-A, -A) > 0)
		{
			gjkSetLine(A, C, pDirection);
		}
		else
		{
			if (AglVector3::dotProduct(B-A, -A) > 0)
			{
				gjkSetLine(A, B, pDirection);
			}
			else
			{
				gjkSetPoint(A, pDirection);
			}
		}
	}
	else
	{
		if (AglVector3::dotProduct(AglVector3::crossProduct(B-A, ABC), -A) > 0)
		{
			if (AglVector3::dotProduct(B-A, -A) > 0)
			{
				gjkSetLine(A, B, pDirection);
			}
			else
			{
				gjkSetPoint(A, pDirection);
			}
		}
		else
		{
			if (AglVector3::dotProduct(ABC, -A) > 0)
			{
				gjkSetTriangle(A, B, C, pDirection);
			}
			else
			{
				gjkSetTriangle(A, C, B, pDirection);
			}
		}
	}
	return false;
}

bool gjkProcessSimplexTetra(AglVector3& pDirection)
{
	AglVector3 A = GJKLIST[3];
	AglVector3 B = GJKLIST[2];
	AglVector3 C = GJKLIST[1];
	AglVector3 D = GJKLIST[0];
	AglVector3 ABC = AglVector3::crossProduct(B-A, C-A);
	AglVector3 ACD = AglVector3::crossProduct(C-A, D-A);
	AglVector3 ADB = AglVector3::crossProduct(D-A, B-A);
	AglVector3 AC = C-A;
	AglVector3 AB = B-A;
	AglVector3 AD = D-A;

	int status = 0;
	if (AglVector3::dotProduct(ABC, -A) > 0)
		status += 1;
	if (AglVector3::dotProduct(ACD, -A) > 0)
		status += 2;
	if (AglVector3::dotProduct(ADB, -A) > 0)
		status += 4;

	switch (status)
	{
		case 0:
		{
			return true;
		} break;
		case 1:
		{
			//In front of ABC only
			GJKLIST[2] = A;
			GJKLIST[1] = B;
			GJKLIST[0] = C;
			GJKSIZE = 3;
			gjkProcessSimplexTriangle(pDirection);
		} break;
		case 2:
		{
			//In front of ACD only
			GJKLIST[2] = A;
			GJKLIST[1] = C;
			GJKLIST[0] = D;
			GJKSIZE = 3;
			gjkProcessSimplexTriangle(pDirection);
		} break;
		case 3:
		{
			//In front of ABC and ACD
			if (AglVector3::dotProduct(AglVector3::crossProduct(ABC, AC), -A) > 0) //Towards ACD
			{
				if (AglVector3::dotProduct(AglVector3::crossProduct(AC, ACD), -A) > 0) //Toward ABC and ACD. Between ABC and ACD
				{
					if (AglVector3::dotProduct(AC, -A) > 0) //Between A and C
					{
						gjkSetLine(A, C, pDirection);
					}
					else //Beyond A
					{
						gjkSetPoint(A, pDirection);
					}
				}
				else //Towards ACD, Away from ABC
				{
					if (AglVector3::dotProduct(AglVector3::crossProduct(ACD, AD), -A) > 0) //Beyond AD line
					{
						gjkSetLine(A, D, pDirection);
					}
					else //Within triangle area
					{
						gjkSetTriangle(A, C, D, pDirection);
					}
				}
			}
			else //Away from ACD
			{
				if (AglVector3::dotProduct(AglVector3::crossProduct(AB, ABC), -A) > 0) //Away from ABC
				{
					if (AglVector3::dotProduct(AB, -A) > 0) //Between A and B
					{
						gjkSetLine(A, B, pDirection);
					}
					else //Beyond A
					{
						gjkSetPoint(A, pDirection);
					}
				}
				else
				{
					gjkSetTriangle(A, B, C, pDirection);
				}
			}
		} break;
		case 4:
		{
			//In front of ADB only
			GJKLIST[2] = A;
			GJKLIST[1] = D;
			GJKLIST[0] = B;
			GJKSIZE = 3;
			gjkProcessSimplexTriangle(pDirection);
		} break;
		case 5:
		{
			//In front of ABC and ADB
			if (AglVector3::dotProduct(AglVector3::crossProduct(ADB, AB), -A) > 0) //Towards ABC
			{
				if (AglVector3::dotProduct(AglVector3::crossProduct(AB, ABC), -A) > 0) //Toward ABC and ADB. Between ABC and ADB
				{
					if (AglVector3::dotProduct(AB, -A) > 0) //Between A and B
					{
						gjkSetLine(A, B, pDirection);
					}
					else //Beyond A
					{
						gjkSetPoint(A, pDirection);
					}
				}
				else //Towards ABC away from ADB
				{
					if (AglVector3::dotProduct(AglVector3::crossProduct(ABC, AC), -A) > 0) //Beyond AC line
					{
						gjkSetLine(A, C, pDirection);
					}
					else //Within triangle area
					{
						gjkSetTriangle(A, B, C, pDirection);
					}
				}
			}
			else //Away from ABC
			{
				if (AglVector3::dotProduct(AglVector3::crossProduct(AD, ADB), -A) > 0) //Beyond ADB
				{
					if (AglVector3::dotProduct(AD, -A) > 0) // Between A and B
					{
						gjkSetLine(A, D, pDirection);
					}
					else //Beyond A
					{
						gjkSetPoint(A, pDirection);
					}
				}
				else
				{
					gjkSetTriangle(A, D, B, pDirection);
				}
			}
		} break;
		case 6:
		{
			//In front of ACD and ADB
			if (AglVector3::dotProduct(AglVector3::crossProduct(ACD, AD), -A) > 0) //Toward ADB
			{
				if (AglVector3::dotProduct(AglVector3::crossProduct(AD, ADB), -A) > 0) //Toward ACD
				{
					if (AglVector3::dotProduct(AD, -A) > 0) //Between A and D
					{
						gjkSetLine(A, D, pDirection);
					}
					else // Beyond A
					{
						gjkSetPoint(A, pDirection);
					}
				}
				else //Toward ADB, away from ACD
				{
					if (AglVector3::dotProduct(AglVector3::crossProduct(ADB, AB), -A) > 0) //Beyond PhyTriangle
					{
						gjkSetLine(A, B, pDirection);
					}
					else //Within PhyTriangle
					{
						gjkSetTriangle(A, D, B, pDirection);
					}
				}
			}
			else //Away from ADB
			{
				if (AglVector3::dotProduct(AglVector3::crossProduct(AC, ACD), -A) > 0) //Beyond PhyTriangle
				{
					if (AglVector3::dotProduct(AC, -A) > 0) //Between A and C
					{
						gjkSetLine(A, C, pDirection);
					}
					else //Beyond A
					{
						gjkSetPoint(A, pDirection);
					}
				}
				else //Within PhyTriangle
				{
					gjkSetTriangle(A, C, D, pDirection);
				}
			}
		} break;
		case 7:
		{
			//In front of all planes - Happens when the origin is "above" the tetrahedron
			if (AglVector3::dotProduct(AB, -A) > 0) //Between A and B
			{
				gjkSetLine(A, B, pDirection);
			}
			else
			{
				if (AglVector3::dotProduct(AC, -A) > 0) //Between A and C
				{
					gjkSetLine(A, C, pDirection);
				}
				else
				{
					if (AglVector3::dotProduct(AD, -A) > 0) //Between A and D
					{
						gjkSetLine(A, D, pDirection);
					}
					else //Beyond A
					{
						gjkSetPoint(A, pDirection);
					}
				}
			}
		} break;
	}

	return false;
}
void gjkSetPoint(AglVector3 pValue, AglVector3& pDirection)
{
	GJKSIZE = 1;
	GJKLIST[0] = pValue;
	pDirection = -pValue;
}
void gjkSetLine(AglVector3 pA, AglVector3 pB, AglVector3& pDirection)
{
	GJKSIZE = 2;
	GJKLIST[0] = pB;
	GJKLIST[1] = pA;
	pDirection = AglVector3::crossProduct(AglVector3::crossProduct(pB-pA, -pA), pB-pA);
}
void gjkSetTriangle(AglVector3 pA, AglVector3 pB, AglVector3 pC, AglVector3& pDirection)
{
	GJKSIZE = 3;
	GJKLIST[0] = pC;
	GJKLIST[1] = pB;
	GJKLIST[2] = pA;
	pDirection = AglVector3::crossProduct(pB-pA, pC-pA);
}

void epaProcessCollision(vector<AglVector3>& pSimplex, const vector<AglVector3>& pA, const vector<AglVector3>& pB, EPACollisionData* pData)
{
	//Construct the simplex
	vector<EPATriangle> simplex = epaConstructTriangleSimplex(pSimplex);

	//Safety: Without this some special case collisions might "fall through the cracks"
	AglVector3 c = (pSimplex[0] + pSimplex[1] + pSimplex[2] + pSimplex[3]) * 0.25f;
	for (unsigned int i = 0; i < simplex.size(); i++)
	{
		if (AglVector3::dotProduct(simplex[i].N, simplex[i].A - c) < 0)
		{
			simplex[i].N = -simplex[i].N;
		}
	}
	while (true)
	{
		float dist;
		int index = epaFindClosestTriangle(simplex, dist);
		if (index == -1)
			return;

		AglVector3 s = gjkSupport(simplex[index].N, pA, pB);

		float distance = AglVector3::dotProduct(s, simplex[index].N);
		if (distance - dist < 0.0001f)
		{
			pData->Normal = simplex[index].N;
			pData->Depth = distance;
			break;
		}

		epaInsertPoint(simplex, s);
	}
}
void epaProcessCollision(vector<AglVector3>& pSimplex, const vector<AglVector3>& pA, const AglBoundingSphere& pSphere, EPACollisionData* pData)
{
	//Construct the simplex
	vector<EPATriangle> simplex = epaConstructTriangleSimplex(pSimplex);
	while (true)
	{
		float dist;
		if (simplex.size() == 0)
		{
			pData->Normal = AglVector3(1, 0, 0);
			pData->Depth = 0;
			break;
		}
		int index = epaFindClosestTriangle(simplex, dist);

		if (_isnan(dist))
		{
			pData->Normal = AglVector3(1, 0, 0);
			pData->Depth = 0;
			break;
		}

		if (index == -1)
			return;

		AglVector3 s = gjkSupport(simplex[index].N, pA, pSphere);

		float distance = AglVector3::dotProduct(s, simplex[index].N);
		if (distance - dist < 0.0001f)
		{
			pData->Normal = simplex[index].N;
			pData->Depth = distance;
			break;
		}

		epaInsertPoint(simplex, s);
	}
}
vector<EPATriangle> epaConstructTriangleSimplex(vector<AglVector3>& pPoints)
{
	vector<EPATriangle> simplex;
	simplex.push_back(EPATriangle(pPoints[2], pPoints[1], pPoints[0]));
	simplex.push_back(EPATriangle(pPoints[3], pPoints[1], pPoints[0]));
	simplex.push_back(EPATriangle(pPoints[2], pPoints[3], pPoints[0]));
	simplex.push_back(EPATriangle(pPoints[2], pPoints[1], pPoints[3]));
	return simplex;
}
int epaFindClosestTriangle(const vector<EPATriangle>& pSimplex, float& pMinDist)
{
	//A simplification is made here. Since the shape is convex
	//it can be assumed that the plane of the closest triangle
	//is the closest triangle as well. This means that a plane-to-point
	//test is sufficient.
	pMinDist = abs(AglVector3::dotProduct(pSimplex[0].A, pSimplex[0].N));
	int triangleIndex = 0;
	for (unsigned int i = 1; i < pSimplex.size(); i++)
	{
		float dist = abs(AglVector3::dotProduct(pSimplex[i].A, pSimplex[i].N));
		if (dist < pMinDist)
		{
			pMinDist = dist;
			triangleIndex = i;
		}
	}
	return triangleIndex;
}
void epaInsertPoint(vector<EPATriangle>& pSimplex, AglVector3 pPoint)
{
	vector<int> seen;
	vector<EPAEdge> edges;
	for (unsigned int i = 0; i < pSimplex.size(); i++)
	{
		if (AglVector3::dotProduct(pPoint - pSimplex[i].A, pSimplex[i].N) > 0)
		{
			seen.push_back(i);
			epaInsertUnique(edges, EPAEdge(pSimplex[i].A, pSimplex[i].B));
			epaInsertUnique(edges, EPAEdge(pSimplex[i].C, pSimplex[i].A));
			epaInsertUnique(edges, EPAEdge(pSimplex[i].B, pSimplex[i].C));
		}
	}

	for (int i = seen.size() - 1; i >= 0; i--)
	{
		pSimplex[seen[i]] = pSimplex.back();
		pSimplex.pop_back();
	}
	for (unsigned int i = 0; i < edges.size(); i++)
	{
		pSimplex.push_back(EPATriangle(edges[i].A, edges[i].B, pPoint));
	}

}
void epaInsertUnique(vector<EPAEdge>& pEdges, EPAEdge pNew)
{
	for (unsigned int i = 0; i < pEdges.size(); i++)
	{
		if (pEdges[i].Equals(pNew))
		{
			pEdges[i] = pEdges.back();
			pEdges.pop_back();
			return;
		}
	}
	pEdges.push_back(pNew);
}

/*void EPATetraFromLine(vector<AglVector3>& pSimplex, const vector<AglVector3>& pA, const vector<AglVector3>& pB)
{
	//Assumes that the simplex contains only two
	//vertices

	AglVector3 d = pSimplex[1] - pSimplex[0];
	AglVector3 e;
	if (abs(d.x) < abs(d.y))
	{
		if (abs(d.x) < abs(d.z))
		{
			e = AglVector3(1, 0, 0);
		}
		else
		{
			e = AglVector3(0, 0, 1);
		}
	}
	else
	{
		if (abs(d.y) < abs(d.z))
		{
			e = AglVector3(0, 1, 0);
		}
		else
		{
			e = AglVector3(0, 0, 1);
		}
	}
	AglVector3 v0 = AglVector3::crossProduct(e, d);
	AglVector3 v1 = v0;
	Rotate(v1, PI * 2.0f / 3.0f, d);
	AglVector3 v2 = v1;
	Rotate(v2, PI * 2.0f / 3.0f, d);
	AglVector3 x0 = GJKSupport(v0, pA, pB);
	AglVector3 x1 = GJKSupport(v1, pA, pB);
	AglVector3 x2 = GJKSupport(v2, pA, pB);

	if (EPAOriginInTetra(pSimplex[0], x0, x1, x2))
	{
		pSimplex.pop_back();
		pSimplex.push_back(x0);
		pSimplex.push_back(x1);
		pSimplex.push_back(x2);
	}
	else //if (EPAOriginInTetra(pSimplex[1], x0, x1, x2))
	{
		pSimplex[0] = pSimplex[1];
		pSimplex.pop_back();
		pSimplex.push_back(x0);
		pSimplex.push_back(x1);
		pSimplex.push_back(x2);
	}

}*/
/*void EPATetraFromTriangle(vector<AglVector3>& pSimplex, const vector<AglVector3>& pA, const vector<AglVector3>& pB)
{
	AglVector3 n = AglVector3::crossProduct(pSimplex[1] - pSimplex[0], pSimplex[2] - pSimplex[0]);
	AglVector3 p1 = GJKSupport(n, pA, pB);
	AglVector3 p2 = GJKSupport(-n, pA, pB);

	if (EPAOriginInTetra(pSimplex[0], pSimplex[1], p1, p2))
	{
		pSimplex[2] = p1;
		pSimplex.push_back(p2);
	}
	else if (EPAOriginInTetra(pSimplex[0], pSimplex[2], p1, p2))
	{
		pSimplex[1] = p1;
		pSimplex.push_back(p2);
	}
	else
	{
		pSimplex[0] = p1;
		pSimplex.push_back(p2);
	}
}*/
/*bool EPAOriginInTetra(AglVector3 pA, AglVector3 pB, AglVector3 pC, AglVector3 pD)
{
	/*AglVector3 n = AglVector3::crossProduct(pB-pA, pC-pA);
	if (AglVector3::dotProduct(n, pA) > 0 == AglVector3::dotProduct(n, pD) > 0)
	{
		return false;
	}
	n = AglVector3::crossProduct(pC-pB, pD-pB);
	if (AglVector3::dotProduct(n, pB) > 0 == AglVector3::dotProduct(n, pA) > 0)
	{
		return false;
	}
	n = AglVector3::crossProduct(pD-pC, pA-pC);
	if (AglVector3::dotProduct(n, pC) > 0 == AglVector3::dotProduct(n, pB) > 0)
	{
		return false;
	}
	n = AglVector3::crossProduct(pA-pD, pB-pD);
	if (AglVector3::dotProduct(n, pC) > 0 == AglVector3::dotProduct(n, pD) > 0)
	{
		return false;
	}
	return true;*/

	/*AglVector3 ABC = AglVector3::crossProduct(pB-pA, pC-pA);
	AglVector3 ACD = AglVector3::crossProduct(pC-pA, pD-pA);
	AglVector3 ADB = AglVector3::crossProduct(pD-pA, pB-pA);
	AglVector3 BCD = AglVector3::crossProduct(pC-pB, pD-pB);

	if (AglVector3::dotProduct(ABC, -pA) > 0)
		return false;
	if (AglVector3::dotProduct(ACD, -pA) > 0)
		return false;
	if (AglVector3::dotProduct(ADB, -pA) > 0)
		return false;
	if (AglVector3::dotProduct(BCD, -pA) > 0)
		return false;
	return true;
}

bool gjkProcessSimplexTetra(vector<AglVector3>& pPointList, AglVector3& pDirection)
{
	AglVector3 A = pPointList[3];
	AglVector3 B = pPointList[2];
	AglVector3 C = pPointList[1];
	AglVector3 D = pPointList[0];

	AglVector3 center = 0.25f * (A + B + C + D);

	//Tri 1 = ACD
	//Tri 2 = BAD
	//Tri 3 = BCA


	//WARNING: Removed normalize: Seems to work. 
	AglVector3 n1 = AglVector3::crossProduct(C-A, D-A);
	AglVector3::normalize(n1);
	AglVector3 n2 = AglVector3::crossProduct(A-B, D-B);
	AglVector3::normalize(n2);
	AglVector3 n3 = AglVector3::crossProduct(C-B, A-B);
	AglVector3::normalize(n3);

	float frac1 = AglVector3::dotProduct(n1, -A);
	float frac2 = AglVector3::dotProduct(n2, -A);
	float frac3 = AglVector3::dotProduct(n3, -A);
	if (frac1 > frac2)
	{
		if (frac1 > frac3)
		{
			if (frac1 > 0)
			{
				pPointList.clear();
				pPointList.push_back(C);
				pPointList.push_back(D);
				pPointList.push_back(A);
				GJKSIZE = 3;
				gjkProcessSimplexTriangle(pDirection);
				return false;
			}
		}
		else if (frac3 > 0)
		{
			pPointList.clear();
			pPointList.push_back(B);
			pPointList.push_back(C);
			pPointList.push_back(A);
			GJKSIZE = 3;
			gjkProcessSimplexTriangle(pDirection);
			return false;
		}
	}
	else if (frac2 > frac3)
	{
		if (frac2 > 0)
		{
			pPointList.clear();
			pPointList.push_back(D);
			pPointList.push_back(B);
			pPointList.push_back(A);
			GJKSIZE = 3;
			gjkProcessSimplexTriangle(pDirection);
			return false;
		}
	}
	else if (frac3 > 0)
	{
		pPointList.clear();
		pPointList.push_back(B);
		pPointList.push_back(C);
		pPointList.push_back(A);
		GJKSIZE = 3;
		gjkProcessSimplexTriangle(pDirection);
		return false;
	}
	return true;
}*/

/*bool gjkInside(AglVector3 pA, AglVector3 pB, AglVector3 pC, AglVector3 pCenter)
{
	AglVector3 n = AglVector3::crossProduct(pB-pA, pC-pA);
	bool v1 = AglVector3::dotProduct(n, pA - pCenter) > 0;
	bool v2 = AglVector3::dotProduct(n, pA) < 0;
	return v1 == v2;
}*/
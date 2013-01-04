#include "AglInteriorSphereGrid.h"

AglVector3 AglInteriorSphereGrid::closestPointOnTriangle(AglVector3 p_p, AglVector3 p_a, 
	AglVector3 p_b, AglVector3 p_c)
{
	//
	//           a     ap
	//           *------------->* p
	//          *   *
	//     ac  *       *   ab
	//        *           *
	//       *               *
	//      *                   *
	//   c * * * * * * * * * * * * * b
	//                bc

	AglVector3 ab = p_b - p_a;
	AglVector3 ac = p_c - p_a;
	AglVector3 bc = p_c - p_b;

	//Factors along ab axis
	float s1 = AglVector3::dotProduct(p_p - p_a, ab);
	float s2 = AglVector3::dotProduct(p_p - p_b, p_a - p_b);

	//Factors along ac axis
	float t1 = AglVector3::dotProduct(p_p - p_a, ac);
	float t2 = AglVector3::dotProduct(p_p - p_c, p_a - p_c);

	//Check if point lies in voronoi region a
	if (s1 <= 0.0f && t1 <= 0.0f)
		return p_a;

	float u1 = AglVector3::dotProduct(p_p - p_b, bc);
	float u2 = AglVector3::dotProduct(p_p - p_c, p_b - p_c);

	//Check if point lies in voronoi region b
	if (s2 <= 0 && u1 <= 0)
		return p_b;

	//Check if point lies in voronoi region c
	if (t2 <= 0 && u2 <= 0)
		return p_c;

	AglVector3 n = AglVector3::crossProduct(p_b - p_a, p_c - p_a);

	//Check if the point lies outside ab and between a and b
	float sideAB = AglVector3::dotProduct(n, AglVector3::crossProduct(p_a - p_p, p_b - p_p));
	if (sideAB <= 0.0f && s1 >= 0 && s2 >= 0)
		return p_a + ab * (s1 / (s1+s2));

	//Check if the point lies outside bc and between b and c
	float sideBC = AglVector3::dotProduct(n, AglVector3::crossProduct(p_b - p_p, p_c - p_p));
	if (sideBC <= 0.0f && u1 >= 0 && u2 >= 0)
		return p_b + bc * (u1 / (u1+u2));

	//Check if the point lies outside ac and between a and c
	float sideAC = AglVector3::dotProduct(n, AglVector3::crossProduct(p_c - p_p, p_a - p_p));
	if (sideAC <= 0.0f && t1 >= 0 && t2 >= 0)
		return p_a + ac * (t1 / (t1+t2));

	//Calculate barycentric
	float u = sideBC / (sideAB + sideBC + sideAC);
	float v = sideAC / (sideAB + sideBC + sideAC);
	float w = 1.0f - u - v;

	return p_a * u + p_b * v + p_c * w;
}

AglInteriorSphere AglInteriorSphereGrid::createSphereAt(AglVector3 p_center, const vector<AglVector3>& p_vertices,
	const vector<unsigned int>& p_indices)
{
	float minDist = FLT_MAX;
	AglVector3 cl;
	for (unsigned int i = 0; i < p_indices.size(); i+=3)
	{
		AglVector3 v1 = p_vertices[p_indices[i]];
		AglVector3 v2 = p_vertices[p_indices[i+1]];
		AglVector3 v3 = p_vertices[p_indices[i+2]];
		AglVector3 closest = closestPointOnTriangle(p_center, v1, v2, v3);
		float dist = AglVector3::lengthSquared(p_center-closest);
		if (dist < minDist)
		{
			minDist = dist;
			cl = closest;
		}
	}
	minDist = sqrt(minDist);
	AglInteriorSphere s;
	s.center = p_center;
	s.radius = minDist;
	return s;
}

AglInteriorSphereGrid::AglInteriorSphereGrid(unsigned int p_gridSize, vector<AglVector3> p_vertices,
	vector<unsigned int> p_indices, unsigned int p_targetMesh)
{
	AglVector3 minP, maxP;
	minP = maxP = p_vertices[0];
	for (unsigned int i = 0; i < p_vertices.size(); i++)
	{
		AglVector3 p = p_vertices[i];
		minP = AglVector3(min(p[0], minP[0]), min(p[1], minP[1]), min(p[2], minP[2]));
		maxP = AglVector3(max(p[0], maxP[0]), max(p[1], maxP[1]), max(p[2], maxP[2]));
	}

	AglVector3 step = (maxP - minP) * (1.0f / p_gridSize);
	AglVector3 start = minP + step * 0.5f;

	m_spheres = new AglInteriorSphere[p_gridSize*p_gridSize*p_gridSize];
	for (unsigned int x = 0; x < p_gridSize; x++)
	{
		for (unsigned int y = 0; y < p_gridSize; y++)
		{
			for (unsigned int z = 0; z < p_gridSize; z++)
			{
				AglVector3 c(start[0] + step[0]*x, start[1] + step[1]*y, start[2] + step[2]*z);
				AglInteriorSphere s = createSphereAt(c, p_vertices, p_indices);
				m_spheres[p_gridSize*p_gridSize * z + p_gridSize * y + x] = s;
			}
		}
	}
	m_header.targetMesh = p_targetMesh;
	m_header.sphereCount = p_gridSize*p_gridSize*p_gridSize;
}
AglInteriorSphereGrid::AglInteriorSphereGrid(AglInteriorSphereGridHeader p_header, 
	AglInteriorSphere* p_spheres)
{
	m_header = p_header;
	m_spheres = p_spheres;
}
AglInteriorSphereGrid::~AglInteriorSphereGrid()
{
	delete[] m_spheres;
}
bool AglInteriorSphereGrid::evaluate(AglVector3 p_center, float p_radius)
{
	for (unsigned int i = 0; i < m_header.sphereCount; i++)
	{
		AglVector3 dir = p_center - m_spheres[i].center;
		float r = p_radius + m_spheres[i].radius;
		if (AglVector3::lengthSquared(dir) < (r-p_radius)*(r-p_radius))
			return true;
	}
	return false;
}

AglInteriorSphereGridHeader AglInteriorSphereGrid::getHeader()
{
	return m_header;
}

AglInteriorSphere* AglInteriorSphereGrid::getSpheres()
{
	return m_spheres;
}
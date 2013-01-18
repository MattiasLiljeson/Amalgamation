#include "AglOBB.h"	
#include "Eigen.h"

AglOBB::AglOBB()
{
	world = AglMatrix::identityMatrix();
	size = AglVector3(0, 0, 0);
}
AglOBB::AglOBB(AglVector3 p_position, AglVector3 p_size)
{
	world		= AglMatrix::identityMatrix();
	world.SetTranslation(p_position);
	size		= p_size;
}
AglOBB::AglOBB(AglMatrix p_world, AglVector3 p_size)
{
	world = p_world;
	size = p_size;
}

vector<AglVector3> AglOBB::getCorners()
{
	vector<AglVector3> c;

	AglVector3 HalfSize = size / 2;
	c.push_back(AglVector3(-HalfSize[0], -HalfSize[1], -HalfSize[2]));
	c.push_back(AglVector3( HalfSize[0], -HalfSize[1], -HalfSize[2]));
	c.push_back(AglVector3(-HalfSize[0],  HalfSize[1], -HalfSize[2]));
	c.push_back(AglVector3(-HalfSize[0], -HalfSize[1],  HalfSize[2]));
	c.push_back(AglVector3( HalfSize[0],  HalfSize[1], -HalfSize[2]));
	c.push_back(AglVector3( HalfSize[0], -HalfSize[1],  HalfSize[2]));
	c.push_back(AglVector3(-HalfSize[0],  HalfSize[1],  HalfSize[2]));
	c.push_back(AglVector3( HalfSize[0],  HalfSize[1],  HalfSize[2]));
	for (int i = 0; i < 8; i++)
	{
		c[i].transform(world);
	}
	return c;
}

//Static Functions
AglOBB AglOBB::constructMinimum(vector<AglVector3> p_vertices, vector<unsigned int> p_indices)
{
	//Compute area of the mesh and centroid of the mesh
	float areaH = 0; //areaH = areaHull
	AglVector3 centerH(0, 0, 0); //centerH = centerHull

	for (unsigned int i = 0; i < p_indices.size(); i+=3)
	{
		AglVector3 v1 = p_vertices[p_indices[i]];
		AglVector3 v2 = p_vertices[p_indices[i+1]];
		AglVector3 v3 = p_vertices[p_indices[i+2]];

		AglVector3 cross = AglVector3::crossProduct(v2-v1, v3-v1);
		
		float area = cross.length() * 0.5f;
		areaH += area;
		centerH += (v1 + v2 + v3) * area / 3;
	}
	centerH /= areaH;

	//Compute the covariance matrix
	float** covariance = new float*[3];
	covariance[0] = new float[3];
	covariance[1] = new float[3];
	covariance[2] = new float[3];
	for (unsigned int i = 0; i < 3; i++)
	{
		for (unsigned int j = 0; j < 3; j++)
		{
			float val = 0;
			for (unsigned int k = 0; k < p_indices.size(); k+=3)
			{
				AglVector3 p = p_vertices[p_indices[k]];
				AglVector3 q = p_vertices[p_indices[k+1]];
				AglVector3 r = p_vertices[p_indices[k+2]];
				AglVector3 m = (p + q + r) / 3;
				AglVector3 cross = AglVector3::crossProduct(q-p, r-p);
				float area = cross.length() * 0.5f;
				val += (area / 12) * (9*m[i]*m[j] + p[i]*p[j] + q[i]*q[j] + r[i]*r[j]); 
			}
			val = val / areaH - centerH[i]*centerH[j];
			covariance[i][j] = val;
		}
	}


	AglVector3 e1, e2, e3;

	FindSymmetricEigenVectors(covariance, e1, e2, e3);

	float minX, maxX, minY, maxY, minZ, maxZ;
	maxX = minX = AglVector3::dotProduct(e1, p_vertices[0]);
	maxY = minY = AglVector3::dotProduct(e2, p_vertices[0]);
	maxZ = minZ = AglVector3::dotProduct(e3, p_vertices[0]);
	for (unsigned int i = 0; i < p_vertices.size(); i++)
	{
		maxX = max(AglVector3::dotProduct(e1, p_vertices[i]), maxX);
		maxY = max(AglVector3::dotProduct(e2, p_vertices[i]), maxY);
		maxZ = max(AglVector3::dotProduct(e3, p_vertices[i]), maxZ);
		minX = min(AglVector3::dotProduct(e1, p_vertices[i]), minX);
		minY = min(AglVector3::dotProduct(e2, p_vertices[i]), minY);
		minZ = min(AglVector3::dotProduct(e3, p_vertices[i]), minZ);
	}

	AglVector3 boxCenter = AglVector3(minX+maxX, minY+maxY, minZ+maxZ) * 0.5f;
	boxCenter = e1 * boxCenter.x + e2 * boxCenter.y + e3 * boxCenter.z;
	AglMatrix w = AglMatrix::identityMatrix();
	w.SetTranslation(boxCenter);
	w.SetRight(e1);
	w.SetUp(e2);
	w.SetForward(e3);

	delete[] covariance[0];
	delete[] covariance[1];
	delete[] covariance[2];
	delete[] covariance;

	return AglOBB(w, AglVector3(maxX-minX, maxY-minY, maxZ-minZ));
}
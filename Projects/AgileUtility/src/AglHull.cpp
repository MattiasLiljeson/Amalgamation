#include "AglHull.h"

AglHull::AglHull(vector<AglVector3> p_points)
{
	//Create the convex hull by using an iterative
	//hull creation solver.


	//Step 1: Create the startup hull using four of the random points
	//In rare cases these points could lie in the same plane or line
	//which causes the algorithm to fail. This can be solved by adding
	//an extra check for this.
	AglVector3 c = AglVector3(0, 0, 0);
	for (int i = 0; i < 4; i++)
	{
		vertices.push_back(p_points.back());
		c += p_points.back();
		p_points.pop_back();
	}
	c *= 0.25f;

	//Step 1: ...cont. Create an initial triangle mesh.
	//Ensure that the triangles are correctly winded.
	addCorrectlyWinded(0, 1, 2, vertices, indices, c);
	addCorrectlyWinded(0, 1, 3, vertices, indices, c);
	addCorrectlyWinded(0, 2, 3, vertices, indices, c);
	addCorrectlyWinded(1, 2, 3, vertices, indices, c);

	while (p_points.size() > 0)
	{
		addPoint(p_points.back(), vertices, indices);
		p_points.pop_back();
	}
}
void AglHull::addCorrectlyWinded(int pI1, int pI2, int pI3, vector<AglVector3> pPoints, vector<unsigned int>& pIndices, AglVector3 pReference)
{
	if (AglVector3::dotProduct(AglVector3::crossProduct(pPoints[pI2] - pPoints[pI1], pPoints[pI3] - pPoints[pI1]), pPoints[pI1] - pReference) > 0)
	{
		pIndices.push_back(pI1);
		pIndices.push_back(pI2);
		pIndices.push_back(pI3);
	}
	else
	{
		pIndices.push_back(pI1);
		pIndices.push_back(pI3);
		pIndices.push_back(pI2);
	}
}
void AglHull::addPoint(AglVector3 pPoint, vector<AglVector3>& pP, vector<unsigned int>& pI)
{
	vector<int> seen;
	vector<pair<int,int>> edges;
	for (unsigned int i = 0; i < pI.size(); i+=3)
	{
		AglVector3 n = AglVector3::crossProduct(pP[pI[i+1]] - pP[pI[i]], pP[pI[i+2]] - pP[pI[i]]);
		if (AglVector3::dotProduct(pPoint - pP[pI[i]], n) >= 0)
		{
			seen.push_back(i);
			insertUnique(edges, pair<int, int>(pI[i], pI[i+1]));
			insertUnique(edges, pair<int, int>(pI[i+2], pI[i]));
			insertUnique(edges, pair<int, int>(pI[i+1], pI[i+2]));
		}
	}
	for (int i = seen.size()-1; i >= 0; i--)
	{
		pI[seen[i]] = pI[pI.size()-3];
		pI[seen[i]+1] = pI[pI.size()-2];
		pI[seen[i]+2] = pI[pI.size()-1];
		pI.pop_back();
		pI.pop_back();
		pI.pop_back();
	}
	if (edges.size() > 0)
	{
		for (unsigned int i = 0; i < edges.size(); i++)
		{
			pI.push_back(edges[i].first);
			pI.push_back(edges[i].second);
			pI.push_back(pP.size());
		}
		pP.push_back(pPoint);
	}
}
void AglHull::insertUnique(vector<pair<int,int>>& pEdges, pair<unsigned int, unsigned int> pNew)
{
	for (unsigned int i = 0; i < pEdges.size(); i++)
	{
		if (pEdges[i].first == pNew.first && pEdges[i].second == pNew.second)
		{
			pEdges[i] = pEdges.back();
			pEdges.pop_back();
			return;
		}
		if (pEdges[i].second == pNew.first && pEdges[i].first == pNew.second) //I don't think this will ever happen if I maintain correct winding
		{
			pEdges[i] = pEdges.back();
			pEdges.pop_back();
			return;
		}
	}
	pEdges.push_back(pNew);
}
// =======================================================================================
//                                      AglHull
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # AglHull
/// Detailed description.....
/// Created on: 14-12-2012 
///---------------------------------------------------------------------------------------
#ifndef AGLHULL_H
#define AGLHULL_H

#include "AglVector3.h"
#include <vector>

using namespace std;

struct AglHull
{
	vector<AglVector3> vertices;
	vector<unsigned int> indices;
	AglHull(vector<AglVector3> p_points);

private:
	void addCorrectlyWinded(int p_i1, int p_i2, int p_i3, vector<AglVector3> p_points, 
								vector<unsigned int>& p_indices, AglVector3 p_reference);
	void addPoint(AglVector3 p_point, vector<AglVector3>& p_p, vector<unsigned int>& p_i);
	void insertUnique(vector<pair<int,int>>& pEdges, pair<unsigned int, unsigned int> pNew);
};

#endif // AGLHULL_H
// =======================================================================================
//                                      AglAABB
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # AglAABB
/// Detailed description.....
/// Created on: 4-12-2012 
///---------------------------------------------------------------------------------------
#ifndef AGLAABB_H
#define AGLAABB_H

#include "AglVector3.h"
#include <vector>

using namespace std;

struct AglAABB
{
	AglVector3 minimum;
	AglVector3 maximum;

	AglAABB(AglVector3 p_p1, AglVector3 p_p2);

	static AglAABB constructMinimum(vector<AglVector3> p_points);
};

#endif // AGLAABB_H
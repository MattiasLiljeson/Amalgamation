// =======================================================================================
//                                      AglOBB
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # OBB
/// Detailed description.....
/// Created on: 4-12-2012 
///---------------------------------------------------------------------------------------
#ifndef AGLOBB_H
#define AGLOBB_H

#include "AglVector3.h"
#include "AglMatrix.h"
#include <vector>

using namespace std;

struct AglOBB
{
	AglMatrix world;
	AglVector3 size;

	AglOBB();
	AglOBB(AglVector3 p_position, AglVector3 p_size);
	AglOBB(AglMatrix p_world, AglVector3 p_size);

	vector<AglVector3> getCorners();

	//Static Functions
	static AglOBB constructMinimum(vector<AglVector3> p_vertices, vector<unsigned int> p_indices);
};

#endif
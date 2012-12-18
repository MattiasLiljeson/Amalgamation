// =======================================================================================
//                                      AglSphereMesh
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # AglSphereMesh
/// Detailed description.....
/// Created on: 14-12-2012 
///---------------------------------------------------------------------------------------
#ifndef AGLSPHEREMESH_H
#define AGLSPHEREMESH_H

#include "AglVector3.h"
#include "AglVector2.h"
#include <vector>

using namespace std;

struct AglSphereMesh
{
	vector<AglVector3> positions;
	vector<AglVector3> normals;
	vector<AglVector2> texCoord;
	vector<unsigned int> indices;

	AglSphereMesh();
	virtual ~AglSphereMesh();
};

#endif // AGLSPHEREMESH_H
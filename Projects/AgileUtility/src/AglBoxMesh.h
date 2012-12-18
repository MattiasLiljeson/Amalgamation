// =======================================================================================
//                                      AglBoxMesh
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # AglBoxMesh
/// Detailed description.....
/// Created on: 14-12-2012 
///---------------------------------------------------------------------------------------
#ifndef AGLBOXMESH_H
#define AGLBOXMESH_H

#include "AglVector3.h"
#include "AglVector2.h"
#include <vector>

using namespace std;

struct AglBoxMesh
{
	vector<AglVector3> positions;
	vector<AglVector3> normals;
	vector<AglVector2> texCoord;
	vector<unsigned int> indices;

	AglBoxMesh();
	virtual ~AglBoxMesh();
};

#endif // AGLBOXMESH_H
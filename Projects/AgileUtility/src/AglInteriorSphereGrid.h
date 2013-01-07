// =======================================================================================
//                                      AglInteriorSphereGrid
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Defines a grid of spheres that is not colliding with geometry in a mesh
///        
/// # AglInteriorSphereGrid
/// Last modified: 28-11-2012 
///---------------------------------------------------------------------------------------
#ifndef AGLINTERIORSPHEREGRID_H
#define AGLINTERIORSPHEREGRID_H

#include "AglVector3.h"
#include <vector>

using namespace std;

struct AglInteriorSphere
{
	AglVector3	center;
	float		radius;
};

struct AglInteriorSphereGridHeader
{
	unsigned int targetMesh;
	unsigned int sphereCount;
};

class AglInteriorSphereGrid
{
private:
	AglInteriorSphereGridHeader m_header;
	AglInteriorSphere* m_spheres;
private:
	AglVector3 closestPointOnTriangle(AglVector3 p_, AglVector3 p_a, 
										AglVector3 p_b, AglVector3 p_c);
	AglInteriorSphere createSphereAt(AglVector3 p_center, 
										const vector<AglVector3>& p_vertices,
											const vector<unsigned int>& p_indices);
public:
	AglInteriorSphereGrid(unsigned int p_gridSize, vector<AglVector3> p_vertices,
		vector<unsigned int> p_indices, unsigned int p_targetMesh);
	AglInteriorSphereGrid(AglInteriorSphereGridHeader p_header, 
							AglInteriorSphere* p_spheres);
	virtual ~AglInteriorSphereGrid();
	bool evaluate(AglVector3 p_center, float p_radius);

	AglInteriorSphereGridHeader getHeader();

	AglInteriorSphere* getSpheres();


};

#endif // AGLINTERIORSPHEREGRID_H
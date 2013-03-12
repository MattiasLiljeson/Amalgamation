// =======================================================================================
//                                      AglCollision
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # AglCollision
/// Detailed description.....
/// Created on: 11-1-2013 
///---------------------------------------------------------------------------------------
#ifndef AGLCOLLISION_H
#define AGLCOLLISION_H

#include "AglOBB.h"
#include "AglBoundingSphere.h"

class AglCollision
{
public:
	static bool isColliding(AglBoundingSphere p_sphere1, AglBoundingSphere p_sphere2);
	static bool isColliding(AglBoundingSphere p_sphere, AglOBB p_obb);
	static bool isColliding(AglOBB p_obb1, AglOBB p_obb2);

	static float overlapAmount(const vector<AglVector3>& p_points1, const vector<AglVector3>& p_points2, const AglVector3& p_axis);
	static void  calculateProjectionInterval(const vector<AglVector3>& p_points, const AglVector3& p_axis, 
									  float& p_min, float& p_max);
};

#endif // AGLCOLLISION_H
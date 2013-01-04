// =======================================================================================
//                                      AglBoundingSphere
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # AglBoundingSphere
/// Detailed description.....
/// Created on: 4-12-2012 
///---------------------------------------------------------------------------------------
#ifndef AGLBOUNDINGSPHERE_H
#define AGLBOUNDINGSPHERE_H

#include "AglVector3.h"
#include "AglVector2.h"
#include "AglQuaternion.h"
#include "AglVector2.h"
#include "Agl2DLine.h"
#include <vector>

using namespace std;

struct AglBoundingSphere
{
	AglVector3 position;
	float radius;

	AglBoundingSphere();
	AglBoundingSphere(AglVector3 p_position, float p_radius);
	AglBoundingSphere(AglVector3 p_p1, AglVector3 p_p2);
	AglBoundingSphere(AglVector3 p_p1, AglVector3 p_p2, AglVector3 p_p3);
	AglBoundingSphere(AglVector3 p_p1, AglVector3 p_p2, AglVector3 p_p3, AglVector3 p_p4);

	bool pointInSphere(AglVector3 p_point);
	bool pointInSphereMargin(AglVector3 p_point, float p_margin);
	

	static AglBoundingSphere minimumBoundingSphere(const vector<AglVector3>& p_points);
private:
	static AglBoundingSphere welzlComputation(const vector<AglVector3>& p_points, unsigned int p_size, 
												vector<AglVector3>& p_span, unsigned int p_spanSize);


};

#endif // AGLBOUNDINGSPHERE_H
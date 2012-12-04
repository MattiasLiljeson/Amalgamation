#ifndef AGLQUATERNION_H
#define AGLQUATERNION_H

#include "AglVector3.h"

//INCOMPLETE!!!!!!

// =================================================================================================
//                                         AglQuaternion
// =================================================================================================

///-------------------------------------------------------------------------------------------------
/// \brief Specifies a quaternion with appropriate operations and functions.
///
/// 
/// # AglQuaternion
/// Edited By: Anton Andersson, 2012-11-20
///-------------------------------------------------------------------------------------------------
struct AglQuaternion
{
	AglVector3	u;
	float		v;

	AglQuaternion(AglVector3 p_u, float p_v);
	AglQuaternion(float p_x, float p_y, float p_z, float p_w);
	AglQuaternion();

	//Operators
	AglQuaternion operator*(const AglQuaternion& p_other);

	//Construction Functions
	static AglQuaternion identity();
	static AglQuaternion constructFromAngularVelocity(AglVector3 p_angVel);
	static AglQuaternion constructFromAxisAndAngle(AglVector3 p_axis, float p_angle);
	static AglQuaternion rotateToFrom(AglVector3 p_from, AglVector3 p_to);
	
	//Functions
	AglQuaternion conjugate();
	void		  normalize();
	void		  transformVector(AglVector3& p_vector);
};

#endif
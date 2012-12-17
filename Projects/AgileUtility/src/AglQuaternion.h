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
	AglQuaternion operator*(const AglQuaternion& p_other) const;
	AglQuaternion& operator*=(const AglQuaternion& p_other);
	AglQuaternion operator*(const float& p_scalar) const;
	AglQuaternion operator+(const AglQuaternion& p_other) const;

	//Construction Functions
	static AglQuaternion identity();
	static AglQuaternion constructFromAngularVelocity(const AglVector3& p_angVel);
	static AglQuaternion constructFromAxisAndAngle(const AglVector3& p_axis, const float& p_angle);
	static AglQuaternion rotateToFrom(AglVector3 p_from, AglVector3 p_to);
	
	//Functions
	AglQuaternion conjugate();
	void		  normalize();
	void		  transformVector(AglVector3& p_vector);

	//Static functions
	static AglQuaternion lerp(const AglQuaternion& p_q1, const AglQuaternion& p_q2, const float& p_t);
	static AglQuaternion slerp(const AglQuaternion& p_q1, const AglQuaternion& p_q2, const float& p_t);
};

#endif
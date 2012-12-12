#ifndef QUATERNION_H
#define QUATERNION_H

#include "Utility.h"

class Quaternion
{
private:
	AglVector3 m_UVector;
	float m_Scalar;
public:
				Quaternion(AglVector3 U, float Scalar);
	AglVector3		GetUVector();
	float		GetScalar();
	Quaternion  Conjugate();
	void		Rotate(float angle, AglVector3 axis);
	Quaternion operator*(const Quaternion& quat);
};

#endif
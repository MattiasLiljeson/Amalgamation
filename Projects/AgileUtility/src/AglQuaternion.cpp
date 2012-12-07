#include "AglQuaternion.h"

AglQuaternion::AglQuaternion(AglVector3 p_u, float p_v)
{
	u = p_u;
	v = p_v;
}
AglQuaternion::AglQuaternion(float p_x, float p_y, float p_z, float p_w)
{
	u = AglVector3(p_x, p_y, p_z);
	v = p_w;
}
AglQuaternion::AglQuaternion()
{
	u = AglVector3(0, 0, 0);
	v = 1.0f;
}

AglQuaternion AglQuaternion::operator*(const AglQuaternion& p_other)
{
	AglVector3 uvec = p_other.u;
	AglVector3 mulU = AglVector3::crossProduct(this->u, p_other.u) + 
							uvec * this->v + this->u * p_other.v;
	float mulV = this->v*p_other.v - AglVector3::dotProduct(this->u, p_other.u);
	AglQuaternion quat(mulU, mulV); 
	return quat;
}

AglQuaternion AglQuaternion::identity()
{
	return AglQuaternion(0, 0, 0, 1);
}
AglQuaternion AglQuaternion::constructFromAngularVelocity(AglVector3 p_angVel)
{
	float a = AglVector3::length(p_angVel);
	if (a < 0.0000001f)
		return AglQuaternion::identity();
	AglVector3 axis = p_angVel / a;
	float cosV = cos(a * 0.5f);
	float sinV = sin(a * 0.5f);
	AglQuaternion quat(axis * sinV, cosV);
	return quat;
}
AglQuaternion AglQuaternion::constructFromAxisAndAngle(AglVector3 p_axis, float p_angle)
{
	float cosV = cos(p_angle * 0.5f);
	float sinV = sin(p_angle * 0.5f);
	AglQuaternion quat(p_axis * sinV, cosV);
	return quat;
}
AglQuaternion AglQuaternion::rotateToFrom(AglVector3 p_from, AglVector3 p_to)
{
	AglVector3 axis = AglVector3::crossProduct(p_from, p_to);
	
	//Parallell vectors. Any orthogonal vector will do
	if (AglVector3::lengthSquared(axis) == 0)
	{
		if (p_from.x != 0)
			axis = AglVector3(-p_from.y, p_from.x, 0);
		else if (p_from.y != 0)
			axis = AglVector3 (0, -p_from.z, p_from.y);
		else
			axis = AglVector3(1, 0, 0);
	}
	AglVector3::normalize(p_from);
	AglVector3::normalize(p_to);
	AglVector3::normalize(axis);
	float angle = acos(AglVector3::dotProduct(p_from, p_to));
	return constructFromAxisAndAngle(axis, angle);
}

AglQuaternion AglQuaternion::conjugate()
{
	return AglQuaternion(-u, v);
}
void AglQuaternion::normalize()
{
	float frac = 1.0f / (sqrt(u.x * u.x + u.y * u.y + u.z * u.z + v * v));
	u.x *= frac;
	u.y *= frac;
	u.z *= frac;
	v *= frac;
}
void AglQuaternion::transformVector(AglVector3& p_vector)
{
	AglQuaternion quat(p_vector, 0);

	quat = this->operator*(quat);
	quat = quat.operator*(this->conjugate());

	p_vector = quat.u;
}
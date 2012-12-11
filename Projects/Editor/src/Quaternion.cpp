#include "Quaternion.h"

Quaternion::Quaternion(AglVector3 U, float Scalar)
{
	m_UVector = U;
	m_Scalar = Scalar;
}
AglVector3 Quaternion::GetUVector()
{
	return m_UVector;
}
float Quaternion::GetScalar()
{
	return m_Scalar;
}
Quaternion Quaternion::Conjugate()
{
	return Quaternion(-this->m_UVector, this->m_Scalar);
}
void Quaternion::Rotate(float angle, AglVector3 axis)
{
	Normalize(axis);
	Quaternion q(axis*sin(angle/2), cos(angle/2));
	*this = q.operator*(*this);
	*this = operator*(q.Conjugate());
}
Quaternion& Quaternion::operator*(const Quaternion& quat)
{
	return Quaternion(CrossProduct(this->m_UVector, quat.m_UVector) + 
				quat.m_UVector*this->m_Scalar + this->m_UVector*quat.m_Scalar,
					this->m_Scalar*quat.m_Scalar - DotProduct(this->m_UVector, quat.m_UVector)); 
			
}
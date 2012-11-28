#include "Vector3.h"
#include <cmath>
#include <cfloat>

const Vector3 Vector3::m_vecZero = Vector3(0, 0, 0);
const Vector3 Vector3::m_vecMin = Vector3(FLT_MIN, FLT_MIN, FLT_MIN);
const Vector3 Vector3::m_vecMax = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);

Vector3::Vector3()
{
	x = 0;
	y = 0;
	z = 0;
}

Vector3::Vector3( float p_x, float p_y, float p_z )
{
	x = p_x;
	y = p_y;
	z = p_z;
}

Vector3::~Vector3()
{
}

Vector3 Vector3::operator+( const Vector3& p_other ) const
{
	Vector3 result;
	result.x = x + p_other.x;
	result.y = y + p_other.y;
	result.z = z + p_other.z;

	return result;
}

Vector3 Vector3::operator-( const Vector3& p_other ) const
{
	Vector3 result;
	result.x = x - p_other.x;
	result.y = y - p_other.y;
	result.z = z - p_other.z;

	return result;
}

Vector3 Vector3::operator-() const
{
	Vector3 out;
	out.x = -x;
	out.y = -y;
	out.z = -z;

	return out;
}

Vector3 Vector3::operator*( const float& p_factor ) const
{
	Vector3 result;
	result.x = x * p_factor;
	result.y = y * p_factor;
	result.z = z * p_factor;

	return result;
}

Vector3 Vector3::operator/( const float& p_denominator ) const
{
	Vector3 result;
	result.x = x / p_denominator;
	result.y = y / p_denominator;
	result.z = z / p_denominator;

	return result;
}

float Vector3::dot( const Vector3& p_other ) const
{
	float result = 0;
	result += x * p_other.x;
	result += y * p_other.y;
	result += z * p_other.z;

	return result;
}

Vector3 Vector3::cross( const Vector3& p_other ) const
{
	Vector3 result;
	result.x = y*p_other.z - z*p_other.y;
	result.y = z*p_other.x - x*p_other.z;
	result.z = x*p_other.y - y*p_other.x;

	return result;
}

void Vector3::normalize()
{
	float len = length();
	x = x / len;
	y = y / len;
	z = z / len;
}

float Vector3::length() const
{
	return sqrtf(x*x + y*y + z*z);
}

float Vector3::lengthSquared() const
{
	return x*x + y*y + z*z;
}

Vector3 Vector3::vecZero()
{
	return m_vecZero;
}

Vector3 Vector3::vecMin()
{
	return m_vecMin;
}

Vector3 Vector3::vecMax()
{
	return m_vecMax;
}
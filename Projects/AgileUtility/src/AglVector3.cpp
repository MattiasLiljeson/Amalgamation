#include "AglVector3.h"
#include "AglMatrix.h"
#include "AglVector4.h"

//Constructors
AglVector3::AglVector3(float p_x, float p_y, float p_z) : x(p_x), y(p_y), z(p_z)
{
}
AglVector3::AglVector3() : x(0), y(0), z(0)
{
}

//Destructor
AglVector3::~AglVector3()
{
}

//Operators
float& AglVector3::operator[](const int& pIndex)
{
	if (pIndex == 0)
		return x;
	else if (pIndex == 1)
		return y;
	return z;
}
const float AglVector3::operator[](const int& pIndex) const
{
	if (pIndex == 0)
		return x;
	else if (pIndex == 1)
		return y;
	return z;
}
AglVector3 AglVector3::operator+(const AglVector3& pOther) const
{
	return AglVector3(x + pOther.x,
					  y + pOther.y,
					  z + pOther.z);
}
AglVector3& AglVector3::operator+=(const AglVector3& pOther)
{
	x += pOther.x;
	y += pOther.y;
	z += pOther.z;
	return (*this);
}
AglVector3 AglVector3::operator-(const AglVector3& pOther) const
{
	return AglVector3(x - pOther.x,
					  y - pOther.y,
					  z - pOther.z);
}
AglVector3& AglVector3::operator-=(const AglVector3& pOther)
{
	x -= pOther.x;
	y -= pOther.y;
	z -= pOther.z;
	return (*this);
}
AglVector3 AglVector3::operator*(const AglVector3& pOther) const
{
	return AglVector3(x * pOther.x,
					  y * pOther.y,
					  z * pOther.z);
}
AglVector3& AglVector3::operator*=(const AglVector3& pOther)
{
	x *= pOther.x;
	y *= pOther.y;
	z *= pOther.z;
	return (*this);
}

AglVector3 AglVector3::operator*(const float& pFactor) const
{
	return AglVector3(x * pFactor,
					  y * pFactor,
					  z * pFactor);
}
AglVector3& AglVector3::operator*=(const float& pFactor)
{
	x *= pFactor;
	y *= pFactor;
	z *= pFactor;
	return (*this);
}
AglVector3 AglVector3::operator/(const float& pFactor) const
{
	float f = 1.0f / pFactor;
	return AglVector3(x * f,
					  y * f,
					  z * f);
}
AglVector3& AglVector3::operator/=(const float& pFactor)
{
	float f = 1.0f / pFactor;
	x *= f;
	y *= f;
	z *= f;
	return (*this);
}
AglVector3 AglVector3::operator-() const 
{
	return AglVector3(-x, -y, -z);
}

bool AglVector3::operator==(const AglVector3& p_other) const
{
	if (x == p_other.x && y == p_other.y && z == p_other.z)
		return true;
	return false;
}

//Member functions
float AglVector3::length()
{
	return sqrt(x*x + y*y + z*z);
}
float AglVector3::lengthSquared()
{
	return x*x + y*y + z*z;
}
void AglVector3::normalize()
{
	float frac = 1.0f / length();
	x *= frac;
	y *= frac;
	z *= frac;
}
void AglVector3::transform(const AglMatrix& pMatrix)
{
	AglVector4 vec = AglVector4(*this, 1);
	vec.transform(pMatrix);
	x = vec.x;
	y = vec.y;
	z = vec.z;
}
void AglVector3::transformNormal(const AglMatrix& pMatrix)
{
	AglVector4 vec = AglVector4(*this, 0);
	vec.transform(pMatrix);
	x = vec.x;
	y = vec.y;
	z = vec.z;
}


//Static functions
float AglVector3::dotProduct(const AglVector3& pV1, const AglVector3& pV2)
{
	return pV1[0] * pV2[0] + pV1[1] * pV2[1] + pV1[2] * pV2[2];
}
AglVector3 AglVector3::crossProduct(const AglVector3& pV1, const AglVector3& pV2)
{
	AglVector3 cross;
	cross[0] = pV1[1] * pV2[2] - pV1[2] * pV2[1];
	cross[1] = pV1[2] * pV2[0] - pV1[0] * pV2[2];
	cross[2] = pV1[0] * pV2[1] - pV1[1] * pV2[0];
	return cross;
}
float AglVector3::lengthSquared(const AglVector3& pVector)
{
	return dotProduct(pVector, pVector);
}
float AglVector3::length(const AglVector3& pVector)
{
	return sqrt(lengthSquared(pVector));
}
void AglVector3::normalize(AglVector3& pVector)
{
	pVector /= (AglVector3::length(pVector));
}
AglVector3 AglVector3::lerp(const AglVector3& p_v1, const AglVector3& p_v2, float p_t)
{
	return p_v1 * (1.0f - p_t) + p_v2 * p_t;
}

AglVector3 AglVector3::createDirection(const AglVector3& p_from, const AglVector3& p_to)
{
	AglVector3 dir = p_to - p_from;
	dir.normalize();
	return dir;
}

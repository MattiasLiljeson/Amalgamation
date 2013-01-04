#include "AglVector4.h"
#include "AglVector3.h"
#include "AglMatrix.h"

//Constructors
AglVector4::AglVector4(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}
AglVector4::AglVector4(AglVector3 vec, float w)
{
	x = vec[0];
	y = vec[1];
	z = vec[2];
	this->w = w;
}
AglVector4::AglVector4()
{
	x = y = z = w = 0;
}

//Destructor
AglVector4::~AglVector4()
{
}

//Operators
float& AglVector4::operator[](const int& pIndex)
{
	if (pIndex == 0)
		return x;
	else if (pIndex == 1)
		return y;
	else if (pIndex == 2)
		return z;
	else
		return w;
}

const float AglVector4::operator[](const int& pIndex) const
{
	if (pIndex == 0)
		return x;
	else if (pIndex == 1)
		return y;
	else if (pIndex == 2)
		return z;
	else
		return w;
}

AglVector4 AglVector4::operator+(const AglVector4& pOther) const
{
	return AglVector4(x + pOther.x, y + pOther.y, z + pOther.z, w + pOther.w);
}

AglVector4& AglVector4::operator+=(const AglVector4& pOther)
{
	x += pOther.x;
	y += pOther.y;
	z += pOther.z;
	w += pOther.w;
	return (*this);
}

AglVector4 AglVector4::operator-(const AglVector4& pOther) const
{
	return AglVector4(x - pOther.x, y - pOther.y, z - pOther.z, w - pOther.w);
}

AglVector4& AglVector4::operator-=(const AglVector4& pOther)
{
	x -= pOther.x;
	y -= pOther.y;
	z -= pOther.z;
	w -= pOther.w;
	return (*this);
}

AglVector4 AglVector4::operator*(const AglVector4& pOther) const
{
	return AglVector4(x * pOther.x,
					  y * pOther.y,
					  z * pOther.z,
					  w * pOther.w);
}

AglVector4& AglVector4::operator*=(const AglVector4& pOther)
{
	x *= pOther.x;
	y *= pOther.y;
	z *= pOther.z;
	w *= pOther.w;
	return (*this);
}

AglVector4 AglVector4::operator*(const float& pFactor) const
{
	return AglVector4(x * pFactor,
					  y * pFactor,
					  z * pFactor,
					  w * pFactor);
}

AglVector4& AglVector4::operator*=(const float& pFactor)
{
	x *= pFactor;
	y *= pFactor;
	z *= pFactor;
	w *= pFactor;
	return (*this);
}

AglVector4 AglVector4::operator/(const float& pFactor) const
{
	float f = 1.0f / pFactor;
	return AglVector4(x * f,
					  y * f,
					  z * f,
					  w * f);
}

AglVector4& AglVector4::operator/=(const float& pFactor)
{
	float f = 1.0f / pFactor;
	x *= f;
	y *= f;
	z *= f;
	w *= f;
	return (*this);
}

AglVector4 AglVector4::operator-() const
{
	return AglVector4(-x, -y, -z, -w);
}

bool AglVector4::operator==(const AglVector4& p_other) const
{
	if (x == p_other.x && y == p_other.y && z == p_other.z && w == p_other.w)
		return true;
	return false;
}

//Member functions
float AglVector4::length() const
{
	return sqrt(x*x + y*y + z*z + w*w);
}
float AglVector4::lengthSquared() const
{
	return x*x + y*y + z*z + w*w;
}
void  AglVector4::normalize()
{
	float frac = 1.0f / length();
	x *= frac;
	y *= frac;
	z *= frac;
	w *= frac;
}
void  AglVector4::transform(AglMatrix pMatrix)
{
	AglMatrix& mat = pMatrix;
	*this =  AglVector4(mat[0] * x + mat[4] * y + mat[8]  * z + mat[12] * w,
						mat[1] * x + mat[5] * y + mat[9]  * z + mat[13] * w,
						mat[2] * x + mat[6] * y + mat[10] * z + mat[14] * w,
						mat[3] * x + mat[7] * y + mat[11] * z + mat[15] * w);
}

//Static functions
float AglVector4::dotProduct(const AglVector4& p_v1, const AglVector4& p_v2)
{
	return p_v1.x*p_v2.x + p_v1.y*p_v2.y + p_v1.z*p_v2.z + p_v1.w*p_v2.w;
}
float AglVector4::lengthSquared(const AglVector4& pVector)
{
	return dotProduct(pVector, pVector);
}
float AglVector4::length(const AglVector4& pVector)
{
	return sqrt(lengthSquared(pVector));
}
void AglVector4::normalize(AglVector4& pVector)
{
	pVector /= length(pVector);
}
AglVector4 AglVector4::lerp(const AglVector4& p_v1, const AglVector4& p_v2, float p_t)
{
	return p_v1 * (1.0f - p_t) + p_v2 * p_t;
}
#include "AglVector2.h"

AglVector2::AglVector2(float x, float y)
{
	this->x = x;
	this->y = y;
}
AglVector2::AglVector2()
{
	x = y = 0;
}

//Destructor
AglVector2::~AglVector2()
{
}

//Operators
float& AglVector2::operator[](const int& pIndex)
{
	if (pIndex == 0)
		return x;
	else
		return y;
}
const float AglVector2::operator[](const int& pIndex) const
{
	if (pIndex == 0)
		return x;
	else
		return y;
}
AglVector2 AglVector2::operator+(const AglVector2& pOther) const
{
	AglVector2 sum(x + pOther.x, y + pOther.y);
	return sum;
}
AglVector2& AglVector2::operator+=(const AglVector2& pOther)
{
	x += pOther.x;
	y += pOther.y;
	return (*this);
}
AglVector2 AglVector2::operator-(const AglVector2& pOther) const
{
	AglVector2 diff(x - pOther.x, y - pOther.y);
	return diff;
}
AglVector2& AglVector2::operator-=(const AglVector2& pOther)
{
	x -= pOther.x;
	y -= pOther.y;
	return (*this);
}
AglVector2 AglVector2::operator*(const AglVector2& pOther) const
{
	AglVector2 product(x * pOther.x, y * pOther.y);
	return product;
}
AglVector2& AglVector2::operator*=(const AglVector2& pOther)
{
	x *= pOther.x;
	y *= pOther.y;
	return (*this);
}

AglVector2 AglVector2::operator*(const float& pFactor) const
{
	AglVector2 product(x * pFactor, y * pFactor);
	return product;
}
AglVector2& AglVector2::operator*=(const float& pFactor)
{
	x *= pFactor;
	y *= pFactor;
	return (*this);
}

	//Static functions
float AglVector2::dotProduct(const AglVector2& pV1, const AglVector2& pV2)
{
	return pV1.x * pV2.x + pV1.y * pV2.y;
}
float AglVector2::crossProduct(const AglVector2& pV1, const AglVector2& pV2)
{
	return pV1[0] * pV2[1] - pV1[1] * pV2[0];
}
float AglVector2::lengthSquared(AglVector2 pVector)
{
	return dotProduct(pVector, pVector);
}
float AglVector2::length(AglVector2 pVector)
{
	return sqrt(lengthSquared(pVector));
}
AglVector2 AglVector2::lerp(const AglVector2& p_v1, const AglVector2& p_v2, float p_t)
{
	return p_v1 * (1.0f - p_t) + p_v2 * p_t;
}
#include "AglVector2.h"

AglVector2::AglVector2(float x, float y)
{
	data[0] = x;
	data[1] = y;
}
AglVector2::AglVector2()
{
	data[0] = data[1] = 0;
}

//Destructor
AglVector2::~AglVector2()
{
}

//Operators
float& AglVector2::operator[](const int& pIndex)
{
	return data[pIndex];
}
const float AglVector2::operator[](const int& pIndex) const
{
	return data[pIndex];
}
AglVector2 AglVector2::operator+(const AglVector2& pOther)
{
	AglVector2 sum;
	for (int i = 0; i < 2; i++)
		sum[i] = data[i] + pOther[i];
	return sum;
}
AglVector2& AglVector2::operator+=(const AglVector2& pOther)
{
	for (int i = 0; i < 2; i++)
		data[i] += pOther[i];
	return (*this);
}
AglVector2 AglVector2::operator-(const AglVector2& pOther)
{
	AglVector2 diff;
	for (int i = 0; i < 2; i++)
		diff[i] = data[i] - pOther[i];
	return diff;
}
AglVector2& AglVector2::operator-=(const AglVector2& pOther)
{
	for (int i = 0; i < 2; i++)
		data[i] -= pOther[i];
	return (*this);
}
AglVector2 AglVector2::operator*(const AglVector2& pOther)
{
	AglVector2 product;
	for (int i = 0; i < 2; i++)
	{
		product[i] = data[i] * pOther.data[i];
	}
	return product;
}
AglVector2& AglVector2::operator*=(const AglVector2& pOther)
{
	AglVector2 product;
	for (int i = 0; i < 2; i++)
	{
		data[i] *= pOther.data[i];
	}
	return (*this);
}

AglVector2 AglVector2::operator*(const float& pFactor)
{
	AglVector2 product;
	for (int i = 0; i < 2; i++)
		product.data[i] = pFactor * data[i];
	return product;
}
AglVector2& AglVector2::operator*=(const float& pFactor)
{
	for (int i = 0; i < 2; i++)
		data[i] *= pFactor;
	return (*this);
}

	//Static functions
float AglVector2::dotProduct(const AglVector2& pV1, const AglVector2& pV2)
{
	return pV1[0] * pV2[0] + pV1[1] * pV2[1];
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
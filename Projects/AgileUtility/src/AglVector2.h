#ifndef AGLVECTOR2_H
#define AGLVECTOR2_H

#include <cmath>

// =================================================================================================
//                                         AglVector2
// =================================================================================================

///-------------------------------------------------------------------------------------------------
/// \brief Specifies a vector2 with appropriate operations and functions.
///
/// 
/// # AglVector2
/// Edited By: Anton Andersson, 2012-11-20
///-------------------------------------------------------------------------------------------------
struct AglVector2
{
	float data[2];

	//Constructors
	AglVector2(float x, float y);
	AglVector2();

	//Destructor
	~AglVector2();

	//Operators
	float& operator[](const int& pIndex);
	const float operator[](const int& pIndex) const;
	AglVector2 operator+(const AglVector2& pOther);
	AglVector2& operator+=(const AglVector2& pOther);
	AglVector2 operator-(const AglVector2& pOther);
	AglVector2& operator-=(const AglVector2& pOther);
	AglVector2 operator*(const AglVector2& pOther);
	AglVector2& operator*=(const AglVector2& pOther);

	AglVector2 operator*(const float& pFactor);
	AglVector2& operator*=(const float& pFactor);

	//Static functions
	static float dotProduct(const AglVector2& pV1, const AglVector2& pV2);
	static float crossProduct(const AglVector2& pV1, const AglVector2& pV2);
	static float lengthSquared(AglVector2 pVector);
	static float length(AglVector2 pVector);
};

#endif
#ifndef AGLVECTOR3_H
#define AGLVECTOR3_H

#include <cmath>

// =================================================================================================
//                                         AglVector3
// =================================================================================================

///-------------------------------------------------------------------------------------------------
/// \brief Specifies a Vector3 with appropriate operations and functions.
///
/// 
/// # AglVector3
/// Edited By: Anton Andersson, 2012-11-20
///-------------------------------------------------------------------------------------------------
struct AglVector3
{
	float x, y, z;

	//Constructors
	AglVector3(float p_x, float p_y, float p_z);
	AglVector3();

	//Destructor
	~AglVector3();

	//Operators
	float& operator[](const int& pIndex);

	const float operator[](const int& pIndex) const;

	AglVector3 operator+(const AglVector3& pOther) const;

	AglVector3& operator+=(const AglVector3& pOther);

	AglVector3 operator-(const AglVector3& pOther) const;

	AglVector3& operator-=(const AglVector3& pOther);

	AglVector3 operator*(const AglVector3& pOther) const;

	AglVector3& operator*=(const AglVector3& pOther);

	AglVector3 operator*(const float& pFactor) const;

	AglVector3& operator*=(const float& pFactor);

	AglVector3 operator/(const float& pFactor) const;

	AglVector3& operator/=(const float& pFactor);

	AglVector3 operator-() const;

	bool operator==(const AglVector3& p_other) const;


	//Static functions
	static float dotProduct(const AglVector3& pV1, const AglVector3& pV2);
	static AglVector3 crossProduct(const AglVector3& pV1, const AglVector3& pV2);
	static float lengthSquared(AglVector3 pVector);
	static float length(AglVector3 pVector);
	static void normalize(AglVector3& pVector);
	
};

#endif
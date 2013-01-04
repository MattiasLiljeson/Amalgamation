#ifndef AGLVECTOR4_H
#define AGLVECTOR4_H

#include <cmath>

//INCOMPLETE!!!!!!

// =================================================================================================
//                                         AglVector4
// =================================================================================================

///-------------------------------------------------------------------------------------------------
/// \brief Specifies a Vector4 with appropriate operations and functions.
///
/// 
/// # AglVector4
/// Edited By: Anton Andersson, 2012-11-20
///-------------------------------------------------------------------------------------------------

struct AglVector3;
struct AglMatrix;

struct AglVector4
{
	float x, y, z, w;

	//Constructors
	AglVector4(float x, float y, float z, float w);
	AglVector4(AglVector3 vec, float w);
	AglVector4();

	//Destructor
	~AglVector4();

	//Operators
	float& operator[](const int& pIndex);

	const float operator[](const int& pIndex) const;

	AglVector4 operator+(const AglVector4& pOther) const;

	AglVector4& operator+=(const AglVector4& pOther);

	AglVector4 operator-(const AglVector4& pOther) const;

	AglVector4& operator-=(const AglVector4& pOther);

	AglVector4 operator*(const AglVector4& pOther) const;

	AglVector4& operator*=(const AglVector4& pOther);

	AglVector4 operator*(const float& pFactor) const;

	AglVector4& operator*=(const float& pFactor);

	AglVector4 operator/(const float& pFactor) const;

	AglVector4& operator/=(const float& pFactor);

	AglVector4 operator-() const;

	bool operator==(const AglVector4& p_other) const;

	//Member functions
	float length() const;
	float lengthSquared() const;
	void  normalize();
	void  transform(AglMatrix pMatrix);

	//Static functions
	static float dotProduct(const AglVector4& p_v1, const AglVector4& p_v2);
	static float lengthSquared(const AglVector4& pVector);
	static float length(const AglVector4& pVector);
	static void normalize(AglVector4& pVector);
	static AglVector4 lerp(const AglVector4& p_v1, const AglVector4& p_v2, float p_t);


};

#endif
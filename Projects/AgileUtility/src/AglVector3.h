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

struct AglMatrix;

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

	//Member functions
	float length();
	float lengthSquared();
	void normalize();
	void transform(const AglMatrix& pMatrix);
	void transformNormal(const AglMatrix& pMatrix);

	//Static functions
	static float dotProduct(const AglVector3& pV1, const AglVector3& pV2);
	static AglVector3 crossProduct(const AglVector3& pV1, const AglVector3& pV2);
	static float lengthSquared(const AglVector3& pVector);
	static float length(const AglVector3& pVector);
	static void normalize(AglVector3& pVector);
	static AglVector3 lerp(const AglVector3& p_v1, const AglVector3& p_v2, float p_t);
	static AglVector3 createDirection(const AglVector3& p_from, const AglVector3& p_to);
	static AglVector3 minOf(const AglVector3& p_v1, const AglVector3& p_v2);
	static AglVector3 maxOf(const AglVector3& p_v1, const AglVector3& p_v2);

	static AglVector3 right();
	static AglVector3 left();
	static AglVector3 up();
	static AglVector3 down();
	static AglVector3 forward();
	static AglVector3 backward();
	static AglVector3 one();
	static AglVector3 zero();
	
};

#endif
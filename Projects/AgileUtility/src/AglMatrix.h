#ifndef AGLMATRIX_H
#define AGLMATRIX_H

#include <cmath>
#include "AglVector3.h"
#include "AglVector2.h"
#include "AglVector4.h"
#include "AglQuaternion.h"

// =================================================================================================
//                                         AglMatrix
// =================================================================================================

///-------------------------------------------------------------------------------------------------
/// \brief Specifies a 4x4 matrix with appropriate operations and functions.
///
/// 
/// # AglMatrix
/// Edited By: Anton Andersson, 2012-11-20
///-------------------------------------------------------------------------------------------------
struct AglMatrix
{
	float data[16];

	//Constructors 
	AglMatrix();
	AglMatrix(float p11, float p12, float p13, float p14,
				float p21, float p22, float p23, float p24,
					float p31, float p32, float p33, float p34,
						float p41, float p42, float p43, float p44);
	AglMatrix(float* pData);

	//Destructor
	~AglMatrix();

	//Operators
	float& operator[](const int& pIndex);
	const float operator[](const int& pIndex) const;
	AglMatrix operator+(const AglMatrix& pOther);
	AglMatrix& operator+=(const AglMatrix& pOther);
	AglMatrix operator-(const AglMatrix& pOther);
	AglMatrix& operator-=(const AglMatrix& pOther);
	AglMatrix operator*(const AglMatrix& pOther);
	AglMatrix& operator*=(const AglMatrix& pOther);

	AglMatrix operator*(const float& pFactor);
	AglMatrix& operator*=(const float& pFactor);

	//Static Functions

	//Returns an identity 4x4matrix
	static AglMatrix identityMatrix();

	//Returns a 4x4matrix filled with zeroes
	static AglMatrix zeroMatrix();

	//Returns the determinant of a soucre 4x4matrix
	static float determinant(AglMatrix pSource);

	//Returns the transpose of a source 4x4matrix
	static AglMatrix transpose(AglMatrix pSource);

	//Returns the inverse of a source 4x4matrix
	static AglMatrix inverse(AglMatrix pSource);

	//Breaks a matrix down into its components
	static void matrixToComponents(AglMatrix pMatrix, AglVector3& pScale, AglQuaternion& pQuaternion, AglVector3& pTranslation);

	//Assembles a matrix from components
	static void componentsToMatrix(AglMatrix& pMatrix, const AglVector3& pScale, const AglQuaternion& pQuaternion, const AglVector3& pTranslation);
};

#endif
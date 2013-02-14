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
	AglMatrix(const AglVector3& pScale, const AglQuaternion& pQuaternion, const AglVector3& pTranslation);

	//Destructor
	~AglMatrix();

	//Operators
	float& operator[](const int& pIndex);
	const float operator[](const int& pIndex) const;
	AglMatrix operator+(const AglMatrix& pOther) const;
	AglMatrix& operator+=(const AglMatrix& pOther);
	AglMatrix operator-(const AglMatrix& pOther) const;
	AglMatrix& operator-=(const AglMatrix& pOther);
	AglMatrix operator*(const AglMatrix& pOther) const;
	AglMatrix& operator*=(const AglMatrix& pOther);

	AglMatrix operator*(const float& pFactor) const;
	AglMatrix& operator*=(const float& pFactor);

	bool operator==(const AglMatrix& pOther) const;


	//Member Getters
	AglVector3 GetForward() const;
	AglVector3 GetBackward() const;
	AglVector3 GetRight() const;
	AglVector3 GetLeft() const;
	AglVector3 GetUp() const;
	AglVector3 GetDown() const;
	AglVector3 GetTranslation() const;
	AglQuaternion GetRotation() const;
	AglVector3 GetScale() const;

	//Member Setters
	void SetForward(const AglVector3& pValue);
	void SetBackward(const AglVector3& pValue);
	void SetRight(const AglVector3& pValue);
	void SetLeft(const AglVector3& pValue);
	void SetUp(const AglVector3& pValue);
	void SetDown(const AglVector3& pValue);
	void SetTranslation(const AglVector3& pValue);

	//Member Functions

	//Returns the determinant of a soucre 4x4matrix
	float determinant() const;

	//Returns the transpose of a source 4x4matrix
	AglMatrix transpose() const;

	//Returns the inverse of a source 4x4matrix
	AglMatrix inverse() const;

	//Breaks a matrix down into its components
	void toComponents(AglVector3& pScale, AglQuaternion& pQuaternion, AglVector3& pTranslation) const;

	//Static Functions

	//Returns an identity 4x4matrix
	static AglMatrix identityMatrix();

	//Returns a 4x4matrix filled with zeroes
	static AglMatrix zeroMatrix();

	//Returns the determinant of a soucre 4x4matrix
	static float determinant(const AglMatrix& pSource);

	//Returns the transpose of a source 4x4matrix
	static AglMatrix transpose(const AglMatrix& pSource);

	//Returns the inverse of a source 4x4matrix
	static AglMatrix inverse(const AglMatrix& pSource);

	//Breaks a matrix down into its components
	static void matrixToComponents(AglMatrix pMatrix, AglVector3& pScale, AglQuaternion& pQuaternion, AglVector3& pTranslation);

	//Assembles a matrix from components
	static void componentsToMatrix(AglMatrix& pMatrix, const AglVector3& pScale, const AglQuaternion& pQuaternion, const AglVector3& pTranslation);

	//Creates a translation matrix
	static AglMatrix createTranslationMatrix(const AglVector3& pTranslation);
	
	//Creates a scale matrix
	static AglMatrix createScaleMatrix(const AglVector3& pScale);

	//Create a rotation matrix
	static AglMatrix createRotationMatrix(const AglQuaternion& pQuaternion);

	//Create a perspective matrix
	static AglMatrix createPerspectiveMatrix(const float& pAspectRatio, const float& pFieldOfView, const float& pNear, const float& pFar);

	//Create a view matrix
	static AglMatrix createViewMatrix(const AglVector3& pPosition, const AglVector3& pTarget, const AglVector3& pUp);
};

#endif
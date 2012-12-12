// =======================================================================================
//                                      Transform
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Transform Component
///        
/// # Transform
/// Detailed description.....
/// Created on: 4-12-2012 
///---------------------------------------------------------------------------------------
#pragma once

#include <AglMatrix.h>
#include <AglVector3.h>
#include <AglQuaternion.h>
#include <Component.h>

//struct xXxVector3 // HACK: DESTROY THIS, and replace with a real one!
//{
//	float x;
//	float y;
//	float z;
//};

class Transform: public Component
{
public:
	///-----------------------------------------------------------------------------------
	/// Sets all private members to 0 (zero).
	/// \returns 
	///-----------------------------------------------------------------------------------
	Transform();

	///-----------------------------------------------------------------------------------
	/// Sets the position. Remaining members are set to 0 (zero).
	/// \param p_posX
	/// \param p_posY
	/// \param p_posZ
	/// \returns 
	///-----------------------------------------------------------------------------------
	Transform( float p_posX, float p_posY, float p_posZ );

	~Transform();

	///-----------------------------------------------------------------------------------
	/// Get a pointer to the translation vector. The data can be manipulated in every way.
	/// \returns A pointer to the translation vector.
	///-----------------------------------------------------------------------------------
	AglVector3* getTranslation();

	///-----------------------------------------------------------------------------------
	/// Get a pointer to the scale vector. The data can be manipulated in every way.
	/// \returns A pointer to the scale vector.
	///-----------------------------------------------------------------------------------
	AglVector3* getScale();

	///-----------------------------------------------------------------------------------
	/// Get a pointer to the scale vector. The data can be manipulated in every way.
	/// \returns A pointer to the rotation quaternion.
	///-----------------------------------------------------------------------------------
	AglQuaternion* getRotation();

	///-----------------------------------------------------------------------------------
	/// Get a pointer to the transform matrix. The data can be manipulated in every way.
	/// \returns A pointer to the transform matrix.
	///----------------------------------------------------------------------------------
	AglMatrix* getMatrix();

	///-----------------------------------------------------------------------------------
	/// Get a pointer to the translated transform matrix.. The data can be manipulated 
	/// in every way.
	/// \returns A pointer to the translated transform matrix.
	///----------------------------------------------------------------------------------
	AglMatrix* getTransposedMatrix();

private:
	///-----------------------------------------------------------------------------------
	/// Calculates the composited matrix for all translation members
	/// \returns void
	///-----------------------------------------------------------------------------------
	void calcCompMatrix();

private:
	AglMatrix m_compositionMatrix;
	AglMatrix m_transposedCompositionMatrix;
	AglVector3 m_translation;
	AglVector3 m_scale;
	AglQuaternion m_rotation;
};
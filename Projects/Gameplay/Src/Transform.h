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
	Transform()
	{
		m_translation.x = 0.0f;
		m_translation.y = 0.0f;
		m_translation.z = 0.0f;

		m_scale.x = 1.0f;
		m_scale.y = 1.0f;
		m_scale.z = 1.0f;

		m_rotation = AglQuaternion::identity();

		//m_orientation.x = 0.0f;
		//m_orientation.y = 0.0f;
		//m_orientation.z = 0.0f;
		
		calcCompMatrix();
	}

	///-----------------------------------------------------------------------------------
	/// Sets the position. Remaining members are set to 0 (zero).
	/// \param p_posX
	/// \param p_posY
	/// \param p_posZ
	/// \returns 
	///-----------------------------------------------------------------------------------
	Transform( float p_posX, float p_posY, float p_posZ )
	{
		m_translation.x = p_posX;
		m_translation.y = p_posY;
		m_translation.z = p_posZ;

		m_scale.x = 1.0f;
		m_scale.y = 1.0f;
		m_scale.z = 1.0f;

		m_rotation = AglQuaternion::identity();

		//m_orientation.x = 0.0f;
		//m_orientation.y = 0.0f;
		//m_orientation.z = 0.0f;

		calcCompMatrix();
	}

	~Transform()
	{
	}

	///-----------------------------------------------------------------------------------
	/// Get a pointer to the translation vector. The data can be manipulated in every way.
	/// \returns A pointer to the translation vector.
	///-----------------------------------------------------------------------------------
	AglVector3* getTranslation()
	{
		return &m_translation;
	}

	///-----------------------------------------------------------------------------------
	/// Get a pointer to the scale vector. The data can be manipulated in every way.
	/// \returns A pointer to the scale vector.
	///-----------------------------------------------------------------------------------
	AglVector3* getScale()
	{
		return &m_scale;
	}

	///-----------------------------------------------------------------------------------
	/// Get a pointer to the scale vector. The data can be manipulated in every way.
	/// \returns A pointer to the rotation quaternion.
	///-----------------------------------------------------------------------------------
	AglQuaternion* getRotation()
	{
		return &m_rotation;
	}

	///-----------------------------------------------------------------------------------
	/// Get a pointer to the scale vector. The data can be manipulated in every way.
	/// \returns A pointer to the transform matrix.
	///----------------------------------------------------------------------------------
	AglMatrix* getMatrix()
	{
		return &m_compositionMatrix;
	}

private:
	///-----------------------------------------------------------------------------------
	/// Calculates the composited matrix for all translation members
	/// \returns void
	///-----------------------------------------------------------------------------------
	void calcCompMatrix()
	{
		AglMatrix::componentsToMatrix(
			m_compositionMatrix,
			m_scale,
			m_rotation,
			m_translation);
	}

private:
	AglMatrix m_compositionMatrix;
	AglVector3 m_translation;
	AglVector3 m_scale;
	AglQuaternion m_rotation;
	///AglVector3 m_orientation;
};
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

#include <Component.h>

struct xXxVector3 // HACK: DESTROY THIS, and replace with a real one!
{
	float x;
	float y;
	float z;
};

class Transform: public Component
{
public:
	Transform()
	{
		m_translation.x = 0.0f;
		m_translation.y = 0.0f;
		m_translation.z = 0.0f;

		m_scale.x = 0.0f;
		m_scale.y = 0.0f;
		m_scale.z = 0.0f;

		m_orientation.x = 0.0f;
		m_orientation.y = 0.0f;
		m_orientation.z = 0.0f;
	}

	Transform( float p_posX, float p_posY, float p_posZ )
	{
		m_translation.x = p_posX;
		m_translation.y = p_posY;
		m_translation.z = p_posZ;

		m_scale.x = 0.0f;
		m_scale.y = 0.0f;
		m_scale.z = 0.0f;

		m_orientation.x = 0.0f;
		m_orientation.y = 0.0f;
		m_orientation.z = 0.0f;
	}

	~Transform()
	{
	}

	xXxVector3* getTranslation()
	{
		return &m_translation;
	}

private:
	xXxVector3 m_translation;
	xXxVector3 m_scale;
	xXxVector3 m_orientation;
};
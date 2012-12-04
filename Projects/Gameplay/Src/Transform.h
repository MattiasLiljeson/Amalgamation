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
		translation.x = 0.0f;
		translation.y = 0.0f;
		translation.z = 0.0f;

		scale.x = 0.0f;
		scale.y = 0.0f;
		scale.z = 0.0f;

		orientation.x = 0.0f;
		orientation.y = 0.0f;
		orientation.z = 0.0f;
	}

	Transform( float p_posX, float p_posY, float p_posZ )
	{
		translation.x = p_posX;
		translation.y = p_posY;
		translation.z = p_posZ;

		scale.x = 0.0f;
		scale.y = 0.0f;
		scale.z = 0.0f;

		orientation.x = 0.0f;
		orientation.y = 0.0f;
		orientation.z = 0.0f;
	}

	~Transform()
	{
	}

	xXxVector3* getTranslation()
	{
		return &translation;
	}

private:
	xXxVector3 translation;
	xXxVector3 scale;
	xXxVector3 orientation;
};
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
	}

	Transform( float p_x, float p_y, float p_z )
	{
		translation.x = p_x;
		translation.y = p_y;
		translation.z = p_z;
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
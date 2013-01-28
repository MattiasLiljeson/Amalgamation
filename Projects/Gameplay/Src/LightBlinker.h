#pragma once
#include <Component.h>
// =======================================================================================
//	LightBlinker
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief description...
///        
/// # LightBlinker
/// Detailed description...
/// Created on: 28-1-2013 
///---------------------------------------------------------------------------------------

class LightBlinker: public Component
{
public:
	LightBlinker(float p_maxRange)
	{
		increase = true;
		maxRange = p_maxRange;
	}
	~LightBlinker(){}

public:
	bool increase;
	float maxRange;
};
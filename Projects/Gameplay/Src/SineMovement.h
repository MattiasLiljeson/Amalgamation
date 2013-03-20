#pragma once
#include <Component.h>
#include <AglVector3.h>
// =======================================================================================
// SineMovement
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief...
///        
/// # SineMovement
/// Detailed description...
/// Created on: 20-3-2013 
///---------------------------------------------------------------------------------------
class SineMovement: public Component
{
public:
	SineMovement();
	SineMovement(AglVector3 p_vector, float p_radian = 0.0f, float p_cycleTime = 1.0f);

public:
	AglVector3 vector;
	float cycleTime;
	float radian;
	AglVector3 originTranslation;

};
#pragma once

#include "Control.h"
#include "InputHelper.h"

class Control;
class InputManager;
class XInputFetcher;

// =======================================================================================
//                                      AnalogueControl
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Implementation of Control for Analogue controls (sticks, triggers) on the 
/// xbox360 controller.
///        
/// This class fetches the latest input data from the first connected 360-controller. the 
/// raw input data (0-255 for triggers, 0-65k for sticks) is normalized to 0.0 - 1.0.
/// Negative values will turn positive if the NEGATIVE SubAxes is used. The raw data will
/// be 0 - 255 for triggers and -32k - 32k for sticks.
/// Created on: 4-12-2012 
/// \author Mattias Liljeson
///---------------------------------------------------------------------------------------

class AnalogueControl : public Control
{
public:
	AnalogueControl( InputHelper::Xbox360Analogs p_axis);
	~AnalogueControl();

	virtual void update( InputManager* p_manager );

private:
	InputHelper::Xbox360Analogs m_axis;
	InputHelper::SubAxes m_subAxis;
};
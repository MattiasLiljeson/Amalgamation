#pragma once

#include "Control.h"
#include "InputHelper.h"
#include "InputManager.h"
#include "XInputFetcher.h"

class Control;
class InputHelper;
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
/// Negative values will turn positive if the NEGATIVE SUB_AXIS is used. The raw data will
/// be 0 - 255 for triggers and -32k - 32k for sticks.
/// Created on: 4-12-2012 
/// \author Mattias Liljeson
///---------------------------------------------------------------------------------------

class AnalogueControl : public Control
{
public:
	AnalogueControl( InputHelper::XBOX360_CONTROLLER_ANALOG p_axis);
	~AnalogueControl();

	virtual void update( InputManager* p_manager );

private:
	InputHelper::XBOX360_CONTROLLER_ANALOG m_axis;
	InputHelper::SUB_AXIS m_subAxis;
};
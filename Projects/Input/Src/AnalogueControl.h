// =======================================================================================
//                                      AnalogueControl
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Implementation of Control for Analogue controls (sticks, triggers) on the 
/// xbox360 controller.
///        
/// # AnalogueControl
/// Detailed description.....
/// Created on: 4-12-2012 
///\author Mattias Liljeson
///---------------------------------------------------------------------------------------
#pragma once

#include "Control.h"
#include "Input.h"
#include "InputManager.h"
#include "XInputFetcher.h"

class Control;
class Input;
class InputManager;
class XInputFetcher;

class AnalogueControl : public Control
{
public:
	AnalogueControl( Input::XBOX360_CONTROLLER_ANALOGS p_axis, Input::SUB_AXIS p_subAxis );
	~AnalogueControl();

	virtual void update( InputManager* p_manager );

private:
	Input::XBOX360_CONTROLLER_ANALOGS m_axis;
	Input::SUB_AXIS m_subAxis;
};


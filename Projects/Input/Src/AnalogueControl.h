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
#include "InputHelper.h"
#include "InputManager.h"
#include "XInputFetcher.h"

class Control;
class InputHelper;
class InputManager;
class XInputFetcher;

class AnalogueControl : public Control
{
public:
	AnalogueControl( InputHelper::XBOX360_CONTROLLER_ANALOGS p_axis, InputHelper::SUB_AXIS p_subAxis );
	~AnalogueControl();

	virtual void update( InputManager* p_manager );

private:
	InputHelper::XBOX360_CONTROLLER_ANALOGS m_axis;
	InputHelper::SUB_AXIS m_subAxis;
};


#pragma once

#include "Control.h"
#include "InputHelper.h"

class InputManager;

// =======================================================================================
//                                      DigitalControl
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Implementation of Control for digital buttons on the xbox360 controller.
///        
/// Status will be set to 1.0 when the button is down and 0.0 when up. The delta will 
/// therefore be 1.0 on the frames where the button is pressed and -1.0 on the frames
/// where the button is released. The raw data will be one of InputHelper::KEY_STATE.
/// Created on: 4-12-2012
///\author Mattias Liljeson
///---------------------------------------------------------------------------------------

class DigitalControl : public Control
{
public:
	DigitalControl( InputHelper::Xbox360Digitals p_btn );
	~DigitalControl();

	virtual void update( float p_dt, InputManager* p_manager );
	virtual int getControlEnum();

private:
	InputHelper::Xbox360Digitals m_btn;
};

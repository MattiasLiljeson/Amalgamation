#pragma once

#include "Control.h"
#include "InputHelper.h"
#include "InputManager.h"
#include "XInputFetcher.h"

class InputHelper;
class InputManager;
class XInputFetcher;

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
	DigitalControl( InputHelper::XBOX360_CONTROLLER_DIGITAL p_btn );
	~DigitalControl();

	virtual void update( InputManager* p_manager );

private:
	InputHelper::XBOX360_CONTROLLER_DIGITAL m_btn;
};

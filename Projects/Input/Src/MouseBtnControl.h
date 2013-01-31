#pragma once

#include "Control.h"
#include "InputHelper.h"

class InputManager;

// =======================================================================================
//                                      MouseBtnControl
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Implementation of Control for Mouse buttons using the Message Loop.
/// 
/// __DON'T FORGET TO PUSH MESSAGES FROM THE MESSAGE LOOP TO THE MESSAGE LOOP FETCHER!!!__
///
/// Status will be set to 1.0 when the button is down and 0.0 when up. The delta will 
/// therefore be 1.0 on the frames where the button is pressed and -1.0 on the frames
/// where the button is released. The raw data will be one of InputHelper::KEY_STATE.
/// Created on: 4-12-2012 
///\author Mattias Liljeson
///---------------------------------------------------------------------------------------

class MouseBtnControl : public Control
{
public:
	MouseBtnControl( InputHelper::MouseButtons p_btn );
	virtual ~MouseBtnControl();

	virtual void update( InputManager* p_manager );

private:
	 InputHelper::MouseButtons m_btn;
};


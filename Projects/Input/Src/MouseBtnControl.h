// =======================================================================================
//                                      MouseBtnControl
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Implementation of Control for Mouse buttons using the Message Loop.
///        
/// # MouseBtnControl
/// Detailed description.....
/// Created on: 4-12-2012 
///\author Mattias Liljeson
///---------------------------------------------------------------------------------------
#pragma once

#include "Control.h"
#include "Input.h"
#include "InputManager.h"
#include "MessageLoopFetcher.h"

class MouseBtnControl : public Control
{
public:
	MouseBtnControl( Input::MOUSE_BTNS p_btn );
	virtual ~MouseBtnControl();

	virtual void update( InputManager* p_manager );

private:
	 Input::MOUSE_BTNS m_btn;
};


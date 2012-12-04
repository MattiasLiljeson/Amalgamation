// =======================================================================================
//                                      MouseMoveControl
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Implementation of Control for Mouse movement using the Message Loop.
///        
/// # MouseMoveControl
/// The movement is measured in pixels per frame. These are dividid by SHRT_MAX to 
/// produce a value between 0.0 and 1.0. This may cause the precision to be too low. 
/// Created on: 4-12-2012 
///\author Mattias Liljeson
///---------------------------------------------------------------------------------------

#pragma once
#include "Control.h"
#include "Input.h"
#include "InputManager.h"
#include "MessageLoopFetcher.h"

class Input;
class InputManager;
class MessageLoopFetcher;

class MouseMoveControl : public Control
{
public:
	MouseMoveControl( Input::MOUSE_AXIS p_axis, Input::SUB_AXIS p_subAxis );
	virtual ~MouseMoveControl();

	virtual void update( InputManager* p_manager );

private:
	Input::MOUSE_AXIS m_axis;
	Input::SUB_AXIS m_subAxis;
};


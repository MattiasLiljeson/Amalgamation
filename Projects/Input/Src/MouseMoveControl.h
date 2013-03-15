#pragma once

#include "Control.h"
#include "InputHelper.h"

class InputManager;

// =======================================================================================
//                                      MouseMoveControl
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Implementation of Control for Mouse movement using the Message Loop.
///
/// __DON'T FORGET TO PUSH MESSAGES FROM THE MESSAGE LOOP TO THE MESSAGE LOOP FETCHER!!!__
///
/// The movement is measured in screen pixels per frame. These are divided by SHRT_MAX and 
/// normalized to produce a value between 0.0 and 1.0. The raw data is the number of 
/// pixels. The raw data can be used to get higher precision mouse movement for example an
/// on-screen cursor.
///
/// Created on: 4-12-2012 
///\author Mattias Liljeson
///---------------------------------------------------------------------------------------

class MouseMoveControl : public Control
{
public:
	MouseMoveControl( InputHelper::MouseAxes p_axis );
	virtual ~MouseMoveControl();

	virtual void update( float p_dt, InputManager* p_manager );

private:
	InputHelper::MouseAxes m_axis;
	InputHelper::SubAxes m_subAxis;
};


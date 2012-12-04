// =======================================================================================
//                                      InputControlFactory
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Factory for creating Controls mapped to different inputs.
///        
/// # InputControlFactory
/// Detailed description.....
/// Created on: 4-12-2012 
///\author Mattias Liljeson
///---------------------------------------------------------------------------------------
#pragma once

#include "AnalogueControl.h"
#include "Control.h"
#include "DigitalControl.h"
#include "Input.h"
#include "KeyControl.h"
#include "MouseBtnControl.h"
#include "MouseMoveControl.h"

class AnalogueControl;
class DigitalControl;
class Control;
class DigitalControl;
class KeyControl;

class InputControlFactory
{
public:
	enum InputType
	{ 
		MOUSE_BTN,
		MOUSE_MOVE,
		KEYBOARD_KEY,
		XINPUT_DIGITAL,
		XINPUT_ANALOG
	};

	InputControlFactory();
	~InputControlFactory();

	/// Add a control by listening for output. Non blocking. NOT IMPLEMENTED.
	/// @return NULL if no input has been detected. Pointer to the control if detected.
	//Control* addControlByListening();

	/// Add a control by its type and subtype. NOT IMPLEMENTED.
	/// @return NULL if any of the parameters are wrong. Pointer to the control if
	/// everything is OK.
	//Control* addControlByType( InputType p_type, int p_subType );

	Control* create360controllerAnalog( Input::XBOX360_CONTROLLER_ANALOGS p_axis,
		Input::SUB_AXIS p_subAxis );
	Control* create360controllerDigital( Input::XBOX360_CONTROLLER_DIGITALS p_btn );
	Control* createKeyboardKey( Input::KEYBOARD_KEYS p_key );
	Control* createMouseButton( Input::MOUSE_BTNS p_btn );
	Control* createMouseMovement( Input::MOUSE_AXIS p_axis, Input::SUB_AXIS p_subAxis );

};


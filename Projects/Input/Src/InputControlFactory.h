#pragma once

#include "AnalogueControl.h"
#include "Control.h"
#include "DigitalControl.h"
#include "InputHelper.h"
#include "KeyControl.h"
#include "MouseBtnControl.h"
#include "MouseMoveControl.h"

#include <vector>
#include <utility>

class AnalogueControl;
class DigitalControl;
class Control;
class DigitalControl;
class KeyControl;

using namespace std;

// =======================================================================================
//                                      InputControlFactory
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Factory for creating Controls mapped to different inputs.
///        
/// This factory is quite bare bones and should be fleshed out when needed.
///
/// Created on: 4-12-2012 
///\author Mattias Liljeson
///---------------------------------------------------------------------------------------

class InputControlFactory
{
public:
	InputControlFactory();
	~InputControlFactory();

	/// Add a control by listening for output. Non blocking. NOT IMPLEMENTED.
	/// @return NULL if no input has been detected. Pointer to the control if detected.
	//Control* addControlByListening();

	/// Add a control by its type and subtype. NOT IMPLEMENTED.
	/// @return NULL if any of the parameters are wrong. Pointer to the control if
	/// everything is OK.
	//Control* addControlByType( InputType p_type, int p_subType );

	// Gamepad
	vector<pair<string, Control*>> create360controllerAnalogAll();
	Control* create360controllerAnalog( InputHelper::XBOX360_CONTROLLER_ANALOG p_axis );
	vector<pair<string, Control*>> create360controllerDigitalAll();
	Control* create360controllerDigital( InputHelper::XBOX360_CONTROLLER_DIGITAL p_btn );

	// Mouse & Keyboard
	vector<pair<string, Control*>> createKeysAToZ();
	Control* createKeyboardKey( InputHelper::KEYBOARD_KEY p_key );
	Control* createMouseButton( InputHelper::MOUSE_BTN p_btn );
	Control* createMouseMovement( InputHelper::MOUSE_AXIS p_axis );

};


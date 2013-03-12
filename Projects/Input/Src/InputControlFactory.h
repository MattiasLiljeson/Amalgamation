#pragma once

#include "InputHelper.h"

#include <vector>
#include <utility>

class AnalogueControl;
class Control;
class Cursor;
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

	///-----------------------------------------------------------------------------------
	/// Create Controls for all analogs (sticks/triggers) on the gamepad.
	/// \return vector<pair<string, Control*>> A vector of pairs of strings and controls
	/// where the string denotes a name of the control which is based on its enum 
	/// identifier.
	///-----------------------------------------------------------------------------------
	vector<pair<string, Control*>> createAll360Analogs();

	///-----------------------------------------------------------------------------------
	/// Create a Control mapped to the supplied input.
	/// \param p_axis Stick/trigger enum identifier.
	/// \return Control* A Control allocated on the stack.
	///-----------------------------------------------------------------------------------
	Control* create360Analog( InputHelper::Xbox360Analogs p_axis );

	///-----------------------------------------------------------------------------------
	/// Create Controls for all digitals (buttons/dpad) on the gamepad.
	/// \return vector<pair<string, Control*>> A vector of pairs of strings and controls.
	/// where the string denotes a name of the control which is based on its enum
	/// identifier.
	///-----------------------------------------------------------------------------------
	vector< pair<string, Control*> > createAll360Digitals();


	///-----------------------------------------------------------------------------------
	/// Create a Control mapped to the supplied input.
	/// \param p_btn Button/dpad enum identifier.
	/// \return Control* A Control allocated on the stack.
	///-----------------------------------------------------------------------------------
	Control* create360Digital( InputHelper::Xbox360Digitals p_btn );

	// Mouse & Keyboard
	///-----------------------------------------------------------------------------------
	/// Create Controls for all chars on the keyboard.
	/// \return vector<pair<string, Control*>> A vector of pairs of strings and controls
	/// where the string denotes a name of the control which is based on its enum 
	/// identifier.
	///-----------------------------------------------------------------------------------
	vector< pair<string, Control*> > createAToZ();


	///-----------------------------------------------------------------------------------
	/// Create Controls for all numbers on the keyboard.
	/// \return vector<pair<string, Control*>>
	///-----------------------------------------------------------------------------------
	vector< pair<string, Control*> > create0To9();

	///-----------------------------------------------------------------------------------
	/// Create Controls for all numpad numbers on the keyboard.
	/// \return vector<pair<string, Control*>>
	///-----------------------------------------------------------------------------------
	vector< pair<string, Control*> > createNumpad0ToNumpad9();

	///-----------------------------------------------------------------------------------
	/// Create a Control mapped to the supplied input.
	/// \param p_key Key enum identifier.
	/// \return Control* A Control allocated on the stack.
	///----------------------------------------------------------------------------------
	Control* createKeyboardKey( InputHelper::KeyboardKeys p_key );
	
	vector< pair<string, Control*> > createAllMouseAxes();
	vector< pair<string, Control*> > createAllMouseButtons();

	///-----------------------------------------------------------------------------------
	/// Create a Control mapped to the supplied input.
	/// \param p_btn Mouse button enum identifier.
	/// \return Control* A Control allocated on the stack.
	///----------------------------------------------------------------------------------
	Control* createMouseButton( InputHelper::MouseButtons p_btn );

	///-----------------------------------------------------------------------------------
	/// Create a Control mapped to the supplied input.
	/// \param p_axis Mouse axis enum identifier.
	/// \return Control* A Control allocated on the stack.
	///----------------------------------------------------------------------------------
	Control* createMouseAxis( InputHelper::MouseAxes p_axis );

	// DOESNT WORK!
	//Cursor* createMouseCursor();
	//Cursor* createGamepadCursor();

};


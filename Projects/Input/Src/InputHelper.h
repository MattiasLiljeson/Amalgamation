#pragma once

//#include "Rocket/Core/Input.h"

// =======================================================================================
//                                      InputHelper
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Helper class containing enums and static help functions. 
///
/// The enums in this class are used throughout the input project.
///
/// Some old code used together with libRocket is commented away but left so that it can
/// be used in the future.
/// Created on: 4-12-2012 
///\author Mattias Liljeson
///---------------------------------------------------------------------------------------

class InputHelper
{
	//Sphagnums
public:
	enum KEY_STATE
	{
		NON_EXISTING_STATE = -1,
		KEY_DOWN,
		KEY_PRESSED,
		KEY_UP,
		KEY_RELEASED,
		NUM_KEY_STATES
	};

	enum KEY_RAW_STATE
	{
		NON_EXISTING_RAW_STATE = -1,
		UP,
		DOWN,
		NUM_RAW_KEY_STATES
	};

	enum SUB_AXIS
	{ 
		NON_EXISTING_SUB_AXIS = -1,
		AXIS_POSITIVE,
		AXIS_NEGATIVE,
		NUM_SUB_AXIS
	};

	enum KEYBOARD_KEY
	{
		NON_EXISTING_KEY = -1,
		W,
		A,
		S,
		D,
		L,
		SPACE,
		LCTRL,
		ESC,
		F1,
		F2,
		F3,
		F4,
		NUM_KEYBOARD_KEYS
	};

	enum MOUSE_BTN
	{
		NON_EXISTING_MOUSE_BTN = -1,
		M_LBTN,
		M_MBTN,
		M_RBTN,
		NUM_MOUSE_BTNS
	};
	enum MOUSE_AXIS
	{
		NON_EXISTING_AXIS = -1,
		X,
		Y,
		Z,
		NUM_MOUSE_AXIS
	}; // Z = Scroll wheel

	enum XBOX360_CONTROLLER_DIGITAL
	{
		NON_EXISTING_DIGITAL = -1,
		DPAD_UP,
		DPAD_DOWN,
		DPAD_LEFT,
		DPAD_RIGHT,
		BTN_START,
		BTN_BACK,
		THUMB_PRESS_L,
		THUMB_PRESS_R,
		SHOULDER_PRESS_L,
		SHOULDER_PRESS_R,
		BTN_A,
		BTN_B,
		BTN_X,
		BTN_Y,
		NUM_XBOX360_CONTROLLER_DIGITALS
	};

	enum XBOX360_CONTROLLER_ANALOG
	{
		NON_EXISTING_ANALOG = -1,
		THUMB_LX,
		THUMB_LY,
		THUMB_RX,
		THUMB_RY,
		TRIGGER_L,
		TRIGGER_R,
		NUM_XBOX360_CONTROLLER_ANALOGS
	};



public:
	//InputHelper();
	//virtual ~InputHelper();

	//virtual void update() = 0;
	//virtual int getKeyState( int p_key ) = 0;
	//virtual int getMouseKeyState( int p_key ) = 0;
	//virtual int getMousePos( int p_axis ) = 0;
	//virtual int getMouseTravel( int p_axis ) = 0;

	/// Overrides the default calcState() with a int instead of bool. This is only to.
	/// collect all performance warnings of bool to int conversions to a single place. 
	static InputHelper::KEY_STATE calcState( const KEY_STATE p_oldState,
		const int p_down );

	/// Calculate new state using the old state en the current state of the button/key.
	///\param p_oldState The state of the key/button key before calling this function.
	///\param p_down If the key/button is being pressed right now
	static InputHelper::KEY_STATE calcState( const KEY_STATE p_oldState,
		const bool p_down );

	/// Calculate state using the latest events and the old state
	///\param p_oldState The state of the key/button key before calling this function.
	///\param p_pressed If a event that tells us that the key/button has been pressed
	/// since the last call to this function has been caught.
	///\param p_released If a event that tells us that the key/button has been released
	/// since the last call to this function has been caught.
	static InputHelper::KEY_STATE calcStateFromEvents( const KEY_STATE p_oldState,
		const bool p_pressed, const bool p_released );

	/// Convert state to status/delta format.
	///\param p_state State to convert from
	///\return The corresponding statusDelta of the supplied state
	static double statusDeltaFromState( const KEY_STATE p_state );

	/// Convert state to status/delta format.
	///\param p_state State to convert from
	///\return The corresponding status of the supplied state
	static double statusFromState( const KEY_STATE p_state );

	//Rocket::Core::Input::KeyIdentifier libRocketFromKeys( int m_key );
	
private:
	// libRocket map
	//Rocket::Core::Input::KeyIdentifier libRocketFromKeysMap[NUM_KEYB_KEYS];
};
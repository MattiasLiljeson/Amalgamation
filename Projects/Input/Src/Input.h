// =======================================================================================
//                                      Input
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Helper class containing enums and static help functions. 
///        
/// # Input
///
///
/// Some old code used together with libRocket is commented away but left so that it can
/// be used in the future.
/// Created on: 4-12-2012 
///\author Mattias Liljeson
///---------------------------------------------------------------------------------------
#pragma once

//#include "Rocket/Core/Input.h"

class Input
{
	//Sphagnums
public:
	enum KEY_STATE
	{
		KEY_DOWN,
		KEY_PRESSED,
		KEY_UP,
		KEY_RELEASED,
		NUM_KEY_STATES
	};

	enum KEY_RAW_STATE
	{
		UP,
		DOWN,
		NUM_RAW_KEY_STATES
	};

	enum SUB_AXIS
	{ 
		AXIS_POSITIVE,
		AXIS_NEGATIVE
	};

	enum KEYBOARD_KEYS
	{
		W,
		A,
		S,
		D,
		SPACE,
		LCTRL,
		ESC,
		F1,
		F2,
		F3,
		F4,
		NUM_KEYBOARD_KEYS
	};

	enum MOUSE_BTNS
	{
		M_LBTN,
		M_MBTN,
		M_RBTN,
		NUM_MOUSE_BTNS
	};
	enum MOUSE_AXIS
	{
		X,
		Y,
		Z,
		NUM_MOUSE_AXIS
	}; // Z = Scroll wheel

	enum XBOX360_CONTROLLER_DIGITALS
	{
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

	enum XBOX360_CONTROLLER_ANALOGS
	{
		THUMB_LX,
		THUMB_LY,
		THUMB_RX,
		THUMB_RY,
		TRIGGER_L,
		TRIGGER_R,
		NUM_XBOX360_CONTROLLER_ANALOGS
	};



public:
	//Input();
	//virtual ~Input();

	//virtual void update() = 0;
	//virtual int getKeyState( int p_key ) = 0;
	//virtual int getMouseKeyState( int p_key ) = 0;
	//virtual int getMousePos( int p_axis ) = 0;
	//virtual int getMouseTravel( int p_axis ) = 0;

	/// Overrides the default calcState() with a int instead of bool. This is only to.
	/// collect all performance warnings of bool to int conversions to a single place. 
	static int calcState( const int p_oldState, const int p_down );

	/// Calculate new state using the old state en the current state of the button/key.
	///\param p_oldState The state of the key/button key before calling this function.
	///\param p_down If the key/button is being pressed right now
	static int calcState( const int p_oldState, const bool p_down );

	/// Calculate state using the latest events and the old state
	///\param p_oldState The state of the key/button key before calling this function.
	///\param p_pressed If a event that tells us that the key/button has been pressed
	/// since the last call to this function has been caught.
	///\param p_released If a event that tells us that the key/button has been released
	/// since the last call to this function has been caught.
	static int calcStateFromEvents( const int p_oldState, const bool p_pressed, const bool p_released );

	/// Convert state to status/delta format.
	///\param p_state State to convert from
	///\return The corresponding statusDelta of the supplied state
	static double statusDeltaFromState( const int p_state );

	/// Convert state to status/delta format.
	///\param p_state State to convert from
	///\return The corresponding status of the supplied state
	static double statusFromState( const int p_state );

	//Rocket::Core::Input::KeyIdentifier libRocketFromKeys( int m_key );
	
private:
	// libRocket map
	//Rocket::Core::Input::KeyIdentifier libRocketFromKeysMap[NUM_KEYB_KEYS];
};
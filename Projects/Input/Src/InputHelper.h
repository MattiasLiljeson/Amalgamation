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

	///\brief Enum for enumerating input devices
	/// Instead of having all input/control types share the same number space (each 
	/// control type having its own unique integer) they now share the same integer space
	/// (eg multiple controls have the integer '2' as identifier in the enumeration). This
	/// is to make iteration, array-indexing etc easier for each input type. To 
	/// distinguish between different input one can instead use the device type
	/// enumeration defined below.
	enum INPUT_DEVICE_TYPE
	{ 
		IT_MOUSE_MOVE,
		IT_MOUSE_BTN,
		IT_KEYBOARD,
		IT_XINPUT_DIGITAL,
		IT_XINPUT_ANALOG
	};

	///\brief A general state enumeration used with all digital (on/off) controls. 
	enum KEY_STATE
	{
		NON_EXISTING_STATE = -1,
		KEY_DOWN,
		KEY_PRESSED,
		KEY_UP,
		KEY_RELEASED,
		NUM_KEY_STATES
	};

	///\brief Enumeration used to specify raw read data from digital controls. 
	/// Two variables of this enum can be used to generate the above states.
	enum KEY_RAW_STATE
	{
		NON_EXISTING_RAW_STATE = -1,
		UP,
		DOWN,
		NUM_RAW_KEY_STATES
	};

	///\Internal enum used in controls.
	/// Used to make the controls range from 0.0 - 1.0 instead of -1.0 - 1.0
	enum SUB_AXIS
	{ 
		NON_EXISTING_SUB_AXIS = -1,
		AXIS_POSITIVE,
		AXIS_NEGATIVE,
		NUM_SUB_AXIS
	};

	///\brief All keyboard keys currently supported. 
	/// More keys may be added in the future. 
	/// If so, make sure to update the input fetchers mappings as they depend on this enum.
	enum KEYBOARD_KEY
	{
		NON_EXISTING_KEY = -1,
		KEY_A,
		KEY_B,
		KEY_C,
		KEY_D,
		KEY_E,
		KEY_F,
		KEY_G,
		KEY_H,
		KEY_I,
		KEY_J,
		KEY_K,
		KEY_L,
		KEY_M,
		KEY_N,
		KEY_O,
		KEY_P,
		KEY_Q,
		KEY_R,
		KEY_S,
		KEY_T,
		KEY_U,
		KEY_V,
		KEY_W,
		KEY_X,
		KEY_Y,
		KEY_Z,
		KEY_SPACE,
		KEY_BACKSPACE,
		KEY_RETURN,
		KEY_LCTRL,
		KEY_ESC,
		KEY_F1,
		KEY_F2,
		KEY_F3,
		KEY_F4,
		KEY_0,
		KEY_1,
		KEY_2,
		KEY_3,
		KEY_4,
		KEY_5,
		KEY_6,
		KEY_7,
		KEY_8,
		KEY_9,
		KEY_NUM0,
		KEY_NUM1,
		KEY_NUM2,
		KEY_NUM3,
		KEY_NUM4,
		KEY_NUM5,
		KEY_NUM6,
		KEY_NUM7,
		KEY_NUM8,
		KEY_NUM9,
		NUM_KEYBOARD_KEYS
	};

	///\brief All mouse buttons currently supported. 
	/// More buttons may be added int he future.
	/// If so, make sure to update the input fetchers mappings as they depend on this enum.
	enum MOUSE_BTN
	{
		NON_EXISTING_MOUSE_BTN = -1,
		M_LBTN,
		M_MBTN,
		M_RBTN,
		NUM_MOUSE_BTNS
	};

	///\brief All mouse axises currently supported.
	/// This will likely stay the same as a player seldom has a 4d mouse.
	enum MOUSE_AXIS
	{
		NON_EXISTING_AXIS = -1,
		X_POSITIVE,
		X_NEGATIVE,
		Y_POSITIVE,
		Y_NEGATIVE,
		Z_POSITIVE, // Z = Scroll wheel
		Z_NEGATIVE, // Z = Scroll wheel
		NUM_MOUSE_AXIS
	};

	///\brief The complete listing of all digital buttons on a std xbox360 controller.
	/// There are not anymore physical buttons on this controller. This listing is 
	/// therefore probably never* going to change.
	/// * never say never....
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

	///\brief The complete listing of all analog axes on a std xbox360 controller.
	/// There are not anymore physical sticks/triggers on this controller. This listing is 
	/// therefore probably never* going to change.
	/// * never say never....
	enum XBOX360_CONTROLLER_ANALOG
	{
		NON_EXISTING_ANALOG = -1,
		THUMB_LX_POSITIVE,
		THUMB_LX_NEGATIVE,
		THUMB_LY_POSITIVE,
		THUMB_LY_NEGATIVE,
		THUMB_RX_POSITIVE,
		THUMB_RX_NEGATIVE,
		THUMB_RY_POSITIVE,
		THUMB_RY_NEGATIVE,
		TRIGGER_L,
		TRIGGER_R,
		NUM_XBOX360_CONTROLLER_ANALOGS
	};



public:
	/// Overrides the default calcState() with a int instead of bool. This is only to
	/// collect all performance warnings of bool to int conversions to a single place.
	/// Gives performance warning, therefore commented away.
	//static InputHelper::KEY_STATE calcState( const KEY_STATE p_oldState,
	//	const int p_down );

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
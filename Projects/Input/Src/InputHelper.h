#pragma once

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
	enum InputDeviceTypes
	{ 
		InputDeviceTypes_NON_EXISTING = -1,
		InputDeviceTypes_FIRST,

		InputDeviceTypes_MOUSE_AXIS = InputDeviceTypes_FIRST,
		InputDeviceTypes_MOUSE_BUTTON,
		InputDeviceTypes_KEYBOARD,
		InputDeviceTypes_XINPUT_DIGITAL,
		InputDeviceTypes_XINPUT_ANALOG,

		InputDeviceTypes_LAST = InputDeviceTypes_XINPUT_ANALOG,
		InputDeviceTypes_CNT
	};

	///\brief A general state enumeration used with all digital (on/off) controls. 
	enum KeyStates
	{
		KeyStates_NON_EXISTING = -1,

		KeyStates_KEY_DOWN,
		KeyStates_KEY_PRESSED,
		KeyStates_KEY_UP,
		KeyStates_KEY_RELEASED,

		KeyStates_CNT
	};

	///\brief Enumeration used to specify raw read data from digital controls. 
	/// Two variables of this enum can be used to generate the above states.
	enum RawKeyStates
	{
		RawKeyStates_NON_EXISTING = -1,

		RawKeyStates_UP,
		RawKeyStates_DOWN,

		RawKeyStates_CNT
	};

	///\Internal enum used in controls.
	/// Used to make the controls range from 0.0 - 1.0 instead of -1.0 - 1.0
	enum SubAxes
	{ 
		SubAxes_NON_EXISTING = -1,

		SubAxes_POSITIVE,
		SubAxes_NEGATIVE,

		SubAxes_CNT
	};

	///\brief All keyboard keys currently supported. 
	/// More keys may be added in the future. 
	/// If so, make sure to update the input fetchers mappings as they depend on this enum.
	enum KeyboardKeys
	{
		KeyboardKeys_NON_EXISTING = -1,

		KeyboardKeys_A,
		KeyboardKeys_B,
		KeyboardKeys_C,
		KeyboardKeys_D,
		KeyboardKeys_E,
		KeyboardKeys_F,
		KeyboardKeys_G,
		KeyboardKeys_H,
		KeyboardKeys_I,
		KeyboardKeys_J,
		KeyboardKeys_K,
		KeyboardKeys_L,
		KeyboardKeys_M,
		KeyboardKeys_N,
		KeyboardKeys_O,
		KeyboardKeys_P,
		KeyboardKeys_Q,
		KeyboardKeys_R,
		KeyboardKeys_S,
		KeyboardKeys_T,
		KeyboardKeys_U,
		KeyboardKeys_V,
		KeyboardKeys_W,
		KeyboardKeys_X,
		KeyboardKeys_Y,
		KeyboardKeys_Z,
		KeyboardKeys_LEFT_SHIFT,
		KeyboardKeys_LEFT_CTRL,
		KeyboardKeys_LEFT_ALT,
		KeyboardKeys_SPACE,
		KeyboardKeys_RIGHT_ALT,
		KeyboardKeys_RIGHT_CTRL,
		KeyboardKeys_RIGHT_SHIFT,
		KeyboardKeys_BACKSPACE,
		KeyboardKeys_RETURN,
		KeyboardKeys_PERIOD,
		KeyboardKeys_ESC,
		KeyboardKeys_TAB,
		KeyboardKeys_LEFT,
		KeyboardKeys_UP,
		KeyboardKeys_DOWN,
		KeyboardKeys_RIGHT,
		KeyboardKeys_LEFT_ARROW,
		KeyboardKeys_UP_ARROW,
		KeyboardKeys_DOWN_ARROW,
		KeyboardKeys_RIGHT_ARROW,
		KeyboardKeys_F1,
		KeyboardKeys_F2,
		KeyboardKeys_F3,
		KeyboardKeys_F4,
		KeyboardKeys_0,
		KeyboardKeys_1,
		KeyboardKeys_2,
		KeyboardKeys_3,
		KeyboardKeys_4,
		KeyboardKeys_5,
		KeyboardKeys_6,
		KeyboardKeys_7,
		KeyboardKeys_8,
		KeyboardKeys_9,
		KeyboardKeys_NUMPAD_0,
		KeyboardKeys_NUMPAD_1,
		KeyboardKeys_NUMPAD_2,
		KeyboardKeys_NUMPAD_3,
		KeyboardKeys_NUMPAD_4,
		KeyboardKeys_NUMPAD_5,
		KeyboardKeys_NUMPAD_6,
		KeyboardKeys_NUMPAD_7,
		KeyboardKeys_NUMPAD_8,
		KeyboardKeys_NUMPAD_9,

		KeyboardKeys_CNT
	};

	///\brief All mouse buttons currently supported. 
	/// More buttons may be added int he future.
	/// If so, make sure to update the input fetchers mappings as they depend on this enum.
	enum MouseButtons
	{
		// Raw, read from buffers / devices etc
		MouseButtons_NON_EXISTING = -1,
		MouseButtons_FIRST,

		MouseButtons_0 = MouseButtons_FIRST,
		MouseButtons_1,
		MouseButtons_2,
		// Friendlier names, below is true for DirectInput. Could be other order if read
		// by other API
		MouseButtons_LEFT = MouseButtons_0,
		MouseButtons_RIGHT = MouseButtons_1,
		MouseButtons_MIDDLE = MouseButtons_2,

		MouseButtons_LAST = MouseButtons_MIDDLE,
		MouseButtons_CNT
	};

	///\brief All mouse axises currently supported.
	/// This will likely stay the same as a player seldom has a 4d mouse.
	enum MouseAxes
	{
		MouseAxes_NON_EXISTING = -1,
		MouseAxes_FIRST,

		MouseAxes_X_POSITIVE = MouseAxes_FIRST,
		MouseAxes_X_NEGATIVE,
		MouseAxes_Y_POSITIVE,
		MouseAxes_Y_NEGATIVE,
		MouseAxes_Z_POSITIVE, // Z = Scroll wheel
		MouseAxes_Z_NEGATIVE, // Z = Scroll wheel

		MouseAxes_LAST = MouseAxes_Z_NEGATIVE,
		MouseAxes_CNT
	};

	///\brief The complete listing of all digital buttons on a std xbox360 controller.
	/// There are not anymore physical buttons on this controller. This listing is 
	/// therefore probably never* going to change.
	/// * never say never....
	enum Xbox360Digitals
	{
		Xbox360Digitals_NON_EXISTING = -1,
		Xbox360Digitals_DPAD_UP,
		Xbox360Digitals_DPAD_DOWN,
		Xbox360Digitals_DPAD_LEFT,
		Xbox360Digitals_DPAD_RIGHT,
		Xbox360Digitals_BTN_START,
		Xbox360Digitals_BTN_BACK,
		Xbox360Digitals_THUMB_PRESS_L,
		Xbox360Digitals_THUMB_PRESS_R,
		Xbox360Digitals_SHOULDER_PRESS_L,
		Xbox360Digitals_SHOULDER_PRESS_R,
		Xbox360Digitals_BTN_A,
		Xbox360Digitals_BTN_B,
		Xbox360Digitals_BTN_X,
		Xbox360Digitals_BTN_Y,
		Xbox360Digitals_CNT
	};

	///\brief The complete listing of all analog axes on a std xbox360 controller.
	/// There are not anymore physical sticks/triggers on this controller. This listing is 
	/// therefore probably never* going to change.
	/// * never say never....
	enum Xbox360Analogs
	{
		Xbox360Analogs_NON_EXISTING = -1,
		Xbox360Analogs_THUMB_LX_POSITIVE,
		Xbox360Analogs_THUMB_LX_NEGATIVE,
		Xbox360Analogs_THUMB_LY_POSITIVE,
		Xbox360Analogs_THUMB_LY_NEGATIVE,
		Xbox360Analogs_THUMB_RX_POSITIVE,
		Xbox360Analogs_THUMB_RX_NEGATIVE,
		Xbox360Analogs_THUMB_RY_POSITIVE,
		Xbox360Analogs_THUMB_RY_NEGATIVE,
		Xbox360Analogs_TRIGGER_L,
		Xbox360Analogs_TRIGGER_R,
		Xbox360Analogs_CNT
	};

public:
	/// Overrides the default calcState() with a int instead of bool. This is only to
	/// collect all performance warnings of bool to int conversions to a single place.
	/// Gives performance warning, therefore commented away.
	//static InputHelper::KeyStates calcState( const KeyStates p_oldState,
	//	const int p_down );

	/// Calculate new state using the old state en the current state of the button/key.
	///\param p_oldState The state of the key/button key before calling this function.
	///\param p_down If the key/button is being pressed right now
	static InputHelper::KeyStates calcState( const KeyStates p_oldState,
		const bool p_down );

	/// Calculate state using the latest events and the old state
	///\param p_oldState The state of the key/button key before calling this function.
	///\param p_pressed If a event that tells us that the key/button has been pressed
	/// since the last call to this function has been caught.
	///\param p_released If a event that tells us that the key/button has been released
	/// since the last call to this function has been caught.
	static InputHelper::KeyStates calcStateFromEvents( const KeyStates p_oldState,
		const bool p_pressed, const bool p_released );

	/// Convert state to status/delta format.
	///\param p_state State to convert from
	///\return The corresponding statusDelta of the supplied state
	static double statusDeltaFromState( const KeyStates p_state );

	/// Convert state to status/delta format.
	///\param p_state State to convert from
	///\return The corresponding status of the supplied state
	static double statusFromState( const KeyStates p_state );

	//static Rocket::Core::Input::KeyIdentifier libRocketFromKeys( int m_key );
	
	/// Converts a keyboard key to a character.
	///\param p_keyCode		KeyboardKeys-code to convert from
	///\param p_upperCase	
	///\return The corresponding character code
	static char charFromKeyboardKey(KeyboardKeys p_keyCode, bool p_upperCase=false);

	// Identifier for an unsupported character 
	static const char NONPRINTABLE_CHAR = '\0';

	/// Initializes private class members.
	static void initialize();
private:
	// Character maps. These contain upper and lower case representations that match the
	// KeyboardKeys. There will be duplicated values and excessive characters that doesn't
	// have a graphical representation, as this is an implementation that favours
	// performance over memory consumption.
	static char upperCaseCharacterMap[KeyboardKeys_CNT];
	static char lowerCaseCharacterMap[KeyboardKeys_CNT];

	// libRocket map
	//static Rocket::Core::Input::KeyIdentifier libRocketFromKeysMap[KeyboardKeys_CNT];
};
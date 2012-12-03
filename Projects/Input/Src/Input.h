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
	enum MOUSE
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
	static int calcState( int p_state, bool p_down );
	static int calcStateFromEvents( int p_state, bool p_pressed, bool p_released );

	//Rocket::Core::Input::KeyIdentifier libRocketFromKeys( int m_key );
	
private:
	// libRocket map
	//Rocket::Core::Input::KeyIdentifier libRocketFromKeysMap[NUM_KEYB_KEYS];
};
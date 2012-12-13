#pragma once

// Below line to stop warnings
#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>
#include "InputHelper.h"

#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

// =======================================================================================
//                                      DirectInputFetcher
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief InputHelper fetcher for mouse and keyboard using Direct InputHelper. Not used 
/// at the moment.
///        
/// Detailed description.....
/// Created on: 4-12-2012 
///\author Mattias Liljeson
///---------------------------------------------------------------------------------------

class DirectInputFetcher
{
public:
	DirectInputFetcher( HINSTANCE* hInstance, HWND* hWnd ); // sets up and initializes DirectInput
	~DirectInputFetcher();	//closes DirectInput and releases memory
	void createDikKeyMap();
	void reset();
	void detectInput();    // gets the current input state
	void update();
	//bool getKey( int key );
	virtual InputHelper::KEY_STATE getKeyState( int p_key );
	InputHelper::KEY_STATE getMouseKeyState( int p_key );
	int getMousePos( int axis );
	int getMouseTravel( int p_axis );

private:
	HINSTANCE* hInstance;
	HWND* hWnd;
	LPDIRECTINPUT8 din;    // the pointer to our DirectInput interface
	LPDIRECTINPUTDEVICE8 dinkeyboard;    // the pointer to the keyboard device
	LPDIRECTINPUTDEVICE8 dinmouse;    // the pointer to the mouse device
	BYTE keystate[256];    // the storage for the key-information
	DIMOUSESTATE mousestate;    // the storage for the mouse-information

	int m_mousePos[InputHelper::NUM_MOUSE_AXIS];
	int m_mouseTravel[InputHelper::NUM_MOUSE_AXIS];
	InputHelper::KEY_STATE m_mouseBtns[InputHelper::NUM_MOUSE_BTNS];
	InputHelper::KEY_STATE m_kbKeys[InputHelper::NUM_KEYBOARD_KEYS];
	int m_dikFromKeyMap[InputHelper::NUM_KEYBOARD_KEYS];
};
// =======================================================================================
//                                      DirectInputFetcher
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Input fetcher for mouse and keyboard using Direct Input. Not used at the
/// moment.
///        
/// # DirectInputFetcher
/// Detailed description.....
/// Created on: 4-12-2012 
///\author Mattias Liljeson
///---------------------------------------------------------------------------------------
#pragma once

#include <dinput.h>
#include "Input.h"

#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

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
	virtual int getKeyState( int p_key );
	int getMouseKeyState( int p_key );
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

	int m_mousePos[Input::NUM_MOUSE_AXIS];
	int m_mouseTravel[Input::NUM_MOUSE_AXIS];
	int m_mouseBtns[Input::NUM_MOUSE_BTNS];
	int m_kbKeys[Input::NUM_KEYBOARD_KEYS];
	int m_dikFromKeyMap[Input::NUM_KEYBOARD_KEYS];
};
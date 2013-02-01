#pragma once


// Below line to stop warnings
#define DIRECTINPUT_VERSION 0x0800

#include "IMouseKeyboardFetcher.h"

#include "InputHelper.h"

#include <dinput.h>
#include <wtypesbase.h>
#include <string>

using namespace std;

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif
// =======================================================================================
//                                      DirectInputFetcher
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief InputHelper fetcher for mouse and keyboard using Direct Input.
///        
/// Detailed description.....
/// Created on: 4-12-2012 
///	\author		Mattias Liljeson
///---------------------------------------------------------------------------------------

class DirectInputFetcher : public IMouseKeyboardFetcher
{
public:

	///-----------------------------------------------------------------------------------
	/// DirectInputFetcher constructor
	/// \param p_hInstance Instance handle
	/// \param p_hWnd Window handle
	/// \param onlyForeground True if inputs only are to be recorded while the window has 
	//// focus.
	/// \param exclusive True if all input will be exclusive for this window. This will
	/// hide the cursor
	///-----------------------------------------------------------------------------------
	DirectInputFetcher( HINSTANCE p_hInstance, HWND p_hWnd,
		bool p_onlyForeground, bool p_exclusive );
	~DirectInputFetcher();
	
	void update();
	InputHelper::KeyStates getKeyState( int p_key );
	InputHelper::KeyStates getMouseBtnState( int p_key );
	int getMousePos( int p_axis );
	int getMouseTravel( int p_axis );

private:
	void init();
	void createInterfacesAndDevices();
	void acquireDevices();
	void checkDirectInputHr( HRESULT p_hr, const string &p_file, const string& p_func,
		int p_line );
	void setCooperation();
	void createDikKeyMap();
	void reset();
	void detectInput();    // gets the current input state

private:
	HINSTANCE m_hInstance;
	HWND m_hWnd;
	bool m_onlyForeground;
	bool m_exclusive;

	LPDIRECTINPUT8 m_din;    // the pointer to our DirectInput interface
	LPDIRECTINPUTDEVICE8 m_dinkeyboard;    // the pointer to the keyboard device
	LPDIRECTINPUTDEVICE8 m_dinmouse;    // the pointer to the mouse device
	static const int NUM_KEYS = 255; 
	BYTE m_keystate[NUM_KEYS];    // the storage for the key-information
	DIMOUSESTATE m_mousestate;    // the storage for the mouse-information

	int m_mousePos[InputHelper::MouseAxes_CNT];
	int m_mouseTravel[InputHelper::MouseAxes_CNT];
	InputHelper::KeyStates m_mouseBtns[InputHelper::MouseButtons_CNT];
	InputHelper::KeyStates m_kbKeys[InputHelper::KeyboardKeys_CNT];
	int m_dikFromKeyMap[InputHelper::KeyboardKeys_CNT];
};
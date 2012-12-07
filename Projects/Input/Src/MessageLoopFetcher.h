#pragma once

#include "InputHelper.h"
#include <Windows.h>
#include <deque>

using namespace std;

// =======================================================================================
//                                      MsgAndParams
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Struct used in the MessageLoopFetcher message que.
///        
/// Contains the details of the message.
///
/// Created on: 4-12-2012 
///\author Mattias Liljeson
///--------------------------------------------------------------------------------------

struct MsgAndParams
{
	UINT message;
	WPARAM wParam;
	LPARAM lParam;
};

// =======================================================================================
//                                      MessageLoopFetcher
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief InputHelper fetcher for mouse and keyboard using the Message Loop. REMEMBER TO USE
/// pushToQue() IN THE CALLBACK MESSAGE LOOP! otherwise this class will not work.
///        
/// # MessageLoopFetcher
/// Detailed description.....
/// Created on: 4-12-2012 
///\author Mattias Liljeson
///---------------------------------------------------------------------------------------

class MessageLoopFetcher
{
public:
	MessageLoopFetcher();
	~MessageLoopFetcher();

	void update();
	InputHelper::KEY_STATE getKeyState( int p_key );
	InputHelper::KEY_STATE getMouseBtnState( int p_key );
	int getMousePos( int p_axis );
	int getMouseTravel( int p_axis );
	void processWindowsEvent( MsgAndParams p_msgAndParams );
	void processWindowsEvent( UINT p_message, WPARAM p_wParam, LPARAM p_lParam );
	static void pushToQue( MsgAndParams p_msgAndParams );
	static void pushToQue( UINT p_message, WPARAM p_wParam, LPARAM p_lParam );

private:
	int m_mouseCurrPos[InputHelper::NUM_MOUSE_AXIS];
	int m_mousePrevPos[InputHelper::NUM_MOUSE_AXIS];
	int m_mouseMoveDelta[InputHelper::NUM_MOUSE_AXIS];
	InputHelper::KEY_STATE m_mouseBtnStates[InputHelper::NUM_MOUSE_BTNS];
	InputHelper::KEY_STATE m_keyStates[InputHelper::NUM_KEYBOARD_KEYS];

	bool m_mouseBtnsPressed[InputHelper::NUM_MOUSE_BTNS];
	bool m_mouseBtnsReleased[InputHelper::NUM_MOUSE_BTNS];
	bool m_keysPressed[InputHelper::NUM_KEYBOARD_KEYS];
	bool m_keysReleased[InputHelper::NUM_KEYBOARD_KEYS];

	int m_keyFromCharMap[VK_LCONTROL+1]; //VK_LCONTROL is the last char
	static deque<MsgAndParams> msgQue;
};


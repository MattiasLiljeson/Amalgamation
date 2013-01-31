#pragma once

#include "IMouseKeyboardFetcher.h"
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

class MessageLoopFetcher : public IMouseKeyboardFetcher
{
public:
	MessageLoopFetcher( HINSTANCE p_hInstance, HWND p_hWnd, bool p_resetCursor = false );
	~MessageLoopFetcher();

	void update();
	InputHelper::KeyStates getKeyState( int p_key );
	InputHelper::KeyStates getMouseBtnState( int p_key );
	int getMousePos( int p_axis );
	int getMouseTravel( int p_axis );

	static void pushToQue( MsgAndParams p_msgAndParams );
	static void pushToQue( UINT p_message, WPARAM p_wParam, LPARAM p_lParam );

private:
	void resetStateBuffers();
	void resetCursor();
	void updateStateBuffers();
	void processWindowsEvent( MsgAndParams p_msgAndParams );
	void processWindowsEvent( UINT p_message, WPARAM p_wParam, LPARAM p_lParam );

private:
	HINSTANCE m_hInstance;
	HWND m_hWnd;
	bool m_resetCursor;

	int m_mouseCurrPos[InputHelper::MouseAxes_CNT];
	int m_mousePrevPos[InputHelper::MouseAxes_CNT];
	int m_mouseMoveDelta[InputHelper::MouseAxes_CNT];
	InputHelper::KeyStates m_mouseBtnStates[InputHelper::MouseButtons_CNT];
	InputHelper::KeyStates m_keyStates[InputHelper::KeyboardKeys_CNT];

	bool m_mouseBtnsPressed[InputHelper::MouseButtons_CNT];
	bool m_mouseBtnsReleased[InputHelper::MouseButtons_CNT];
	bool m_keysPressed[InputHelper::KeyboardKeys_CNT];
	bool m_keysReleased[InputHelper::KeyboardKeys_CNT];

	int m_keyFromCharMap[VK_LCONTROL+1]; //VK_LCONTROL is the last char
	static deque<MsgAndParams> msgQue;
};


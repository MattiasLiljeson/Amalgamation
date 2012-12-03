#pragma once
#include "Input.h"
#include <Windows.h>
#include <deque>

using namespace std;

struct MsgAndParams
{
	UINT message;
	WPARAM wParam;
	LPARAM lParam;
};

class MessageLoopFetcher
{
public:
	MessageLoopFetcher();
	~MessageLoopFetcher();

	void update();
	int getKeyState( int p_key );
	int getMouseKeyState( int p_key );
	int getMousePos( int p_axis );
	int getMouseTravel( int p_axis );
	void processWindowsEvent( MsgAndParams p_msgAndParams );
	void processWindowsEvent( UINT p_message, WPARAM p_wParam, LPARAM p_lParam );
	static void pushToQue( MsgAndParams p_msgAndParams );
	static void pushToQue( UINT p_message, WPARAM p_wParam, LPARAM p_lParam );

private:
	int m_mousePos[Input::NUM_MOUSE_AXIS];
	int m_mouseTravel[Input::NUM_MOUSE_AXIS];
	int m_mouseBtnStates[Input::NUM_MOUSE_BTNS];
	int m_keyStates[Input::NUM_KEYBOARD_KEYS];

	bool m_mouseBtnsPressed[Input::NUM_MOUSE_BTNS];
	bool m_mouseBtnsReleased[Input::NUM_MOUSE_BTNS];
	bool m_keysPressed[Input::NUM_KEYBOARD_KEYS];
	bool m_keysReleased[Input::NUM_KEYBOARD_KEYS];

	int m_keyFromCharMap[VK_LCONTROL+1]; //VK_LCONTROL is the last char
	static deque<MsgAndParams> msgQue;
};


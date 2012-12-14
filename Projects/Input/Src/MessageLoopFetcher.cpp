#include "MessageLoopFetcher.h"

deque<MsgAndParams> MessageLoopFetcher::msgQue;

MessageLoopFetcher::MessageLoopFetcher( bool p_resetCursor )
{
	m_resetCursor = p_resetCursor;

	for( int i=0; i<InputHelper::NUM_MOUSE_AXIS+1; i++ )
	{
		m_mouseCurrPos[i] = 0;
		m_mouseMoveDelta[i] = 0;
	}

	for( int i=0; i<InputHelper::MOUSE_BTN::NUM_MOUSE_BTNS; i++ )
	{
		m_mouseBtnStates[i] = InputHelper::KEY_STATE::KEY_UP;
		m_mouseBtnsPressed[i] = false;
		m_mouseBtnsReleased[i] = false;
	}

	for( int i=0; i<InputHelper::KEYBOARD_KEY::NUM_KEYBOARD_KEYS; i++ )
	{
		m_keyStates[i] = InputHelper::KEY_STATE::KEY_UP;
		m_keysPressed[i] = false;
		m_keysReleased[i] = false;
	}
	
	for( int i=0; i<VK_LCONTROL+1; i++ ) //VK_LCONTROL is the last char
		m_keyFromCharMap[i] = -1;

	m_keyFromCharMap['W']			= InputHelper::W;
	m_keyFromCharMap['A']			= InputHelper::A;
	m_keyFromCharMap['S']			= InputHelper::S;
	m_keyFromCharMap['D']			= InputHelper::D;
	m_keyFromCharMap['L']			= InputHelper::L;
	m_keyFromCharMap[VK_SPACE]		= InputHelper::SPACE;
	m_keyFromCharMap[VK_F1]			= InputHelper::F1;
	m_keyFromCharMap[VK_F2]			= InputHelper::F2;
	m_keyFromCharMap[VK_F3]			= InputHelper::F3;
	m_keyFromCharMap[VK_F4]			= InputHelper::F4;
	m_keyFromCharMap[VK_LCONTROL]	= InputHelper::LCTRL;
	m_keyFromCharMap[VK_ESCAPE]		= InputHelper::ESC;
}

MessageLoopFetcher::~MessageLoopFetcher()
{
}

void MessageLoopFetcher::update()
{
	if( m_resetCursor )
	{
		// Reset cursor pos so that the mouse can be freely moved. If the mouse is moved 
		// outside of the window w/o this the window will no longer receive input.
		resetCursor();
	}

	resetStateBuffers();

	// Fetch latest input and refresh buffers
	while(!msgQue.empty())
	{
		processWindowsEvent( msgQue.front() );
		msgQue.pop_front();
	}

	updateStateBuffers();
}

void MessageLoopFetcher::resetStateBuffers()
{
	// Reset per frame buffers
	for( int i=0; i<InputHelper::NUM_MOUSE_BTNS; i++)
	{
		m_mouseBtnsPressed[i] = false;
		m_mouseBtnsReleased[i] = false;
	}
	for( int i=0; i<InputHelper::NUM_KEYBOARD_KEYS; i++)
	{
		m_keysPressed[i] = false;
		m_keysReleased[i] = false;
	}
	for( int i=0; i< InputHelper::NUM_MOUSE_AXIS; i++ )
	{
		m_mousePrevPos[i] = m_mouseCurrPos[i];
	}
}

void MessageLoopFetcher::resetCursor()
{
	// HACK: uses getActiveWindow. Will fail if alt-tabbing. 
	HWND windowHandle = GetActiveWindow();

	// Fetch window position in screen space
	RECT windowPos;
	GetWindowRect( windowHandle, &windowPos );

	POINT point;
	// Set the point to the center of the window:
	int windowSizeX = (windowPos.right - windowPos.left);
	int windowSizeY = (windowPos.bottom - windowPos.top);
	point.x = windowSizeX/2 + windowPos.left;
	point.y = windowSizeY/2 + windowPos.top;
	// Set the cursor to the center of the window
	SetCursorPos( point.x, point.y );
	// Convert screen space coords to client space
	ScreenToClient( windowHandle, &point );

	m_mouseCurrPos[InputHelper::MOUSE_AXIS::X] = point.x;
	m_mouseCurrPos[InputHelper::MOUSE_AXIS::Y] = point.y;
	m_mousePrevPos[InputHelper::MOUSE_AXIS::X] = point.x;
	m_mousePrevPos[InputHelper::MOUSE_AXIS::Y] = point.y;
}

void MessageLoopFetcher::updateStateBuffers()
{
	// Update key states based on buffer states
	for( int i=0; i<InputHelper::NUM_MOUSE_BTNS; i++)
	{
		m_mouseBtnStates[i] = InputHelper::calcStateFromEvents( m_mouseBtnStates[i],
			m_mouseBtnsPressed[i], m_mouseBtnsReleased[i] );
	}

	for( int i=0; i<InputHelper::NUM_KEYBOARD_KEYS; i++)
	{
		m_keyStates[i] = InputHelper::calcStateFromEvents( m_keyStates[i], m_keysPressed[i],
			m_keysReleased[i] );
	}

	for( int i=0; i< InputHelper::NUM_MOUSE_AXIS; i++ )
	{
		m_mouseMoveDelta[i] = m_mouseCurrPos[i] - m_mousePrevPos[i];
	}
}

InputHelper::KEY_STATE MessageLoopFetcher::getKeyState( int p_key )
{
	return m_keyStates[p_key];
}

InputHelper::KEY_STATE MessageLoopFetcher::getMouseBtnState( int p_key )
{
	return m_mouseBtnStates[p_key];
}

int MessageLoopFetcher::getMousePos( int axis )
{
	if( 0 <= axis && axis < InputHelper::NUM_MOUSE_AXIS )
		return m_mouseCurrPos[axis];
	else
		return 0;
}

int MessageLoopFetcher::getMouseTravel( int axis )
{
	if( 0 <= axis && axis < InputHelper::NUM_MOUSE_AXIS )
		return m_mouseMoveDelta[axis];
	else
		return 0;
}

void MessageLoopFetcher::processWindowsEvent( MsgAndParams p_msgAndParams )
{
	processWindowsEvent( p_msgAndParams.message, p_msgAndParams.wParam, p_msgAndParams.lParam );
}

void MessageLoopFetcher::processWindowsEvent( UINT p_message, WPARAM p_wParam, LPARAM p_lParam )
{
	// Process all mouse and keyboard events
	switch (p_message)
	{
	case WM_MOUSEMOVE:
		m_mouseCurrPos[InputHelper::X] = LOWORD(p_lParam);
		m_mouseCurrPos[InputHelper::Y] = HIWORD(p_lParam);
		break;

	case WM_KEYDOWN:
		{
			int key = m_keyFromCharMap[p_wParam];
			if(key != -1)
			{
				m_keysPressed[key] = InputHelper::KEY_RAW_STATE::DOWN;
			}
		}
		break;

	case WM_KEYUP:
		{
			int key = m_keyFromCharMap[p_wParam];
			if(key != -1)
			{
				m_keysPressed[key] = InputHelper::KEY_RAW_STATE::UP;
			}
		}
		break;

	case WM_LBUTTONDOWN:
		m_mouseBtnsPressed[InputHelper::M_LBTN] = true;
		break;

	case WM_LBUTTONUP:
		m_mouseBtnsReleased[InputHelper::M_LBTN] = true;
		break;

	case WM_RBUTTONDOWN:
		m_mouseBtnsPressed[InputHelper::M_MBTN] = true;
		break;

	case WM_RBUTTONUP:
		m_mouseBtnsReleased[InputHelper::M_MBTN] = true;
		break;

	case WM_MBUTTONDOWN:
		m_mouseBtnsPressed[InputHelper::M_MBTN] = true;
		break;

	case WM_MBUTTONUP:
		m_mouseBtnsReleased[InputHelper::M_MBTN] = true;
		break;

	case WM_MOUSEWHEEL:
		m_mouseCurrPos[InputHelper::Z] = HIWORD(p_wParam)/* / -WHEEL_DELTA*/;
		break;

	case WM_CHAR:
		{
			//HACK: Ignore for now

			//// Only send through printable characters.
			//if (w_param >= 32)
			//	context->ProcessTextInput((Rocket::Core::word) w_param);
			//// Or endlines - Windows sends them through as carriage returns.
			//else if (w_param == '\r')
			//	context->ProcessTextInput((Rocket::Core::word) '\n');
		}
		break;
	}
}

void MessageLoopFetcher::pushToQue( UINT p_message, WPARAM p_wParam, LPARAM p_lParam )
{
	MsgAndParams msgAndParams;
	msgAndParams.message = p_message;
	msgAndParams.wParam = p_wParam;
	msgAndParams.lParam = p_lParam;
	pushToQue( msgAndParams );
}

void MessageLoopFetcher::pushToQue( MsgAndParams p_msgAndParams )
{
	msgQue.push_back( p_msgAndParams );
}



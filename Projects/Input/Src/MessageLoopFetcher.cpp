#include "MessageLoopFetcher.h"

deque<MsgAndParams> MessageLoopFetcher::msgQue;

MessageLoopFetcher::MessageLoopFetcher()
{
	for( int i=0; i<Input::NUM_MOUSE_AXIS+1; i++ )
	{
		m_mousePos[i] = 0;
		m_mouseTravel[i] = 0;
	}
	for( int i=0; i<VK_LCONTROL+1; i++ ) //VK_LCONTROL is the last char
		m_keyFromCharMap[i] = -1;

	m_keyFromCharMap['W']			= Input::W;
	m_keyFromCharMap['A']			= Input::A;
	m_keyFromCharMap['S']			= Input::S;
	m_keyFromCharMap['D']			= Input::D;
	m_keyFromCharMap[VK_SPACE]		= Input::SPACE;
	m_keyFromCharMap[VK_F1]			= Input::F1;
	m_keyFromCharMap[VK_F2]			= Input::F2;
	m_keyFromCharMap[VK_F3]			= Input::F3;
	m_keyFromCharMap[VK_F4]			= Input::F4;
	m_keyFromCharMap[VK_LCONTROL]	= Input::LCTRL;
	m_keyFromCharMap[VK_ESCAPE]		= Input::ESC;
}

MessageLoopFetcher::~MessageLoopFetcher()
{
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
		case WM_LBUTTONDOWN:
			m_mouseBtnsPressed[Input::M_LBTN] = true;
			break;

		case WM_LBUTTONUP:
			m_mouseBtnsReleased[Input::M_LBTN] = true;
			break;

		case WM_RBUTTONDOWN:
			m_mouseBtnsPressed[Input::M_MBTN] = true;
			break;

		case WM_RBUTTONUP:
			m_mouseBtnsReleased[Input::M_MBTN] = true;
			break;

		case WM_MBUTTONDOWN:
			m_mouseBtnsPressed[Input::M_MBTN] = true;
			break;

		case WM_MBUTTONUP:
			m_mouseBtnsReleased[Input::M_MBTN] = true;
			break;

		case WM_MOUSEMOVE:
			m_mouseTravel[Input::X] = LOWORD(p_lParam) - m_mousePos[Input::X];
			m_mouseTravel[Input::Y] = HIWORD(p_lParam) - m_mousePos[Input::Y];
			m_mousePos[Input::X] = LOWORD(p_lParam);
			m_mousePos[Input::Y] = HIWORD(p_lParam);

			break;

		case WM_MOUSEWHEEL:
			m_mouseTravel[Input::Z] = HIWORD(p_wParam) / -WHEEL_DELTA;
			break;

		case WM_KEYDOWN:
		{

			int key = m_keyFromCharMap[p_wParam];
			if(key != -1)
				m_keysPressed[key] = Input::DOWN;
		}
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

		case WM_KEYUP:
		{
			int key = m_keyFromCharMap[p_wParam];
			if(key != -1)
				m_keysReleased[key] = Input::UP;
		}
		break;
	}
}

void MessageLoopFetcher::update()
{
	// Reset per frame buffers
	for( int i=0; i<Input::NUM_MOUSE_BTNS; i++)
	{
		m_mouseBtnsPressed[i] = false;
		m_mouseBtnsReleased[i] = false;
	}
	for( int i=0; i<Input::NUM_KEYBOARD_KEYS; i++)
	{
		m_keysPressed[i] = false;
		m_keysReleased[i] = false;
	}

	// Fetch latest input and refresh buffers
	while(!msgQue.empty())
	{
		processWindowsEvent( msgQue.front() );
		msgQue.pop_front();
	}

	// Update key states based on buffer states
	for( int i=0; i<Input::NUM_MOUSE_BTNS; i++)
		m_mouseBtnStates[i] = Input::calcStateFromEvents( m_mouseBtnStates[i],
		m_mouseBtnsPressed[i], m_mouseBtnsReleased[i] );

	for( int i=0; i<Input::NUM_KEYBOARD_KEYS; i++)
		m_keyStates[i] = Input::calcStateFromEvents( m_keyStates[i], m_keysPressed[i],
		m_keysReleased[i] );
}

int MessageLoopFetcher::getKeyState( int p_key )
{
	return m_keyStates[p_key];
}

int MessageLoopFetcher::getMouseKeyState( int p_key )
{
	return m_mouseBtnStates[p_key];
}

int MessageLoopFetcher::getMousePos( int axis )
{
	if( 0 <= axis && axis < Input::NUM_MOUSE_AXIS )
		return m_mousePos[axis];
	else
		return 0;
}

int MessageLoopFetcher::getMouseTravel( int axis )
{
	if( 0 <= axis && axis < Input::NUM_MOUSE_AXIS )
		return m_mouseTravel[axis];
	else
		return 0;
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



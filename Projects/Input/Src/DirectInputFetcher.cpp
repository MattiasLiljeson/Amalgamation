#include "DirectInputFetcher.h"

/************************************************************************/
/* PUBLIC																*/
/************************************************************************/
DirectInputFetcher::DirectInputFetcher(HINSTANCE _hInstance, HWND _hWnd,
									   bool p_onlyForeground, bool p_exclusive )
{
	m_hInstance = _hInstance;
	m_hWnd = _hWnd;
	m_onlyForeground = p_onlyForeground;
	m_exclusive = p_exclusive;

	if( m_hWnd != 0 && m_hInstance != 0 )
	{
		init();
	}
	else
	{
		//FAIL!
		int breakHere = 0;
	}
}

DirectInputFetcher::~DirectInputFetcher()
{
	m_dinkeyboard->Unacquire();    // make sure the keyboard is unacquired
	m_dinmouse->Unacquire();    // make sure the mouse in unacquired
	m_din->Release();    // close DirectInput before exiting
}

void DirectInputFetcher::update()
{
	detectInput();

	// Mouse travel (delta position / movement)
	m_mouseTravel[InputHelper::X] = (int)m_mousestate.lX;
	m_mouseTravel[InputHelper::Y] = (int)m_mousestate.lY;
	m_mouseTravel[InputHelper::Z] = (int)m_mousestate.lZ;

	//HACK: Could be done by using mouseTravel:
	// The mouse position is being fetched not through Direct InputHelper but through
	// the windows api. The z-component (scroll wheel) is fetched through DI.
	POINT cursorPos;
	GetCursorPos(&cursorPos);
	m_mousePos[InputHelper::X] = (int)cursorPos.x;
	m_mousePos[InputHelper::Y] = (int)cursorPos.y;
	m_mousePos[InputHelper::Z] += (int)m_mousestate.lZ;

	for( int i=0; i<InputHelper::NUM_MOUSE_BTNS; i++)
	{
		m_mouseBtns[i] = InputHelper::calcState( m_mouseBtns[i], m_mousestate.rgbButtons[i] );
	}
	for( int i=0; i<InputHelper::NUM_KEYBOARD_KEYS; i++)
	{
		m_kbKeys[i] = InputHelper::calcState( m_kbKeys[i], (m_keystate[m_dikFromKeyMap[i]] & 0x80) );
	}
}

InputHelper::KEY_STATE DirectInputFetcher::getKeyState( int p_key )
{
	if( 0 <= p_key && p_key < InputHelper::NUM_KEYBOARD_KEYS )
		return m_kbKeys[p_key];
	else
		return InputHelper::KEY_UP;
}

InputHelper::KEY_STATE DirectInputFetcher::getMouseBtnState( int p_key )
{
	if( 0 <= p_key && p_key < InputHelper::NUM_MOUSE_BTNS )
		return m_mouseBtns[p_key];
	else
		return InputHelper::KEY_UP;
}

int DirectInputFetcher::getMousePos(int axis)
{
	if( 0 <= axis && axis < InputHelper::NUM_MOUSE_AXIS )
		return m_mousePos[axis];
	else
		return 0;
}

int DirectInputFetcher::getMouseTravel(int axis)
{
	if( 0 <= axis && axis < InputHelper::NUM_MOUSE_AXIS )
		return m_mouseTravel[axis];
	else
		return 0;
}

/************************************************************************/
/* PRIVATE																*/
/************************************************************************/
void DirectInputFetcher::init()
{
	createInterfacesAndDevices();
	setCooperation();
	createDikKeyMap();
	reset();
}

void DirectInputFetcher::createInterfacesAndDevices()
{
	HRESULT hr = S_OK;
	if( FAILED(hr = DirectInput8Create(m_hInstance,    // the handle to the application
		DIRECTINPUT_VERSION,    // the compatible version
		IID_IDirectInput8,    // the DirectInput interface version
		(void**)&m_din,    // the pointer to the interface
		NULL)))    // COM stuff, so we'll set it to NULL
	{
		throw DirectInputException(hr, __FILE__, __FUNCTION__, __LINE__);
	}

	if( FAILED(hr = m_din->CreateDevice(GUID_SysKeyboard,    // the default keyboard ID being used
		&m_dinkeyboard,    // the pointer to the device interface
		NULL)))    // COM stuff, so we'll set it to NULL
	{
		throw DirectInputException(hr, __FILE__, __FUNCTION__, __LINE__);
	}

	if( FAILED(hr = m_din->CreateDevice(GUID_SysMouse,
		&m_dinmouse,
		NULL)))
	{
		throw DirectInputException(hr, __FILE__, __FUNCTION__, __LINE__);
	}

	// set the data format to keyboard format
	m_dinkeyboard->SetDataFormat(&c_dfDIKeyboard);
	m_dinmouse->SetDataFormat(&c_dfDIMouse);
}

void DirectInputFetcher::setCooperation()
{
	DWORD settings = 0;
	if( m_onlyForeground )
	{
		settings |= DISCL_FOREGROUND;
	}
	else
	{
		settings |= DISCL_BACKGROUND;
	}

	if( m_exclusive )
	{
		settings |= DISCL_EXCLUSIVE;
	}
	else
	{
		settings |= DISCL_NONEXCLUSIVE;
	}

	m_dinkeyboard->SetCooperativeLevel( m_hWnd, settings );
	m_dinmouse->SetCooperativeLevel( m_hWnd, settings );
}

void DirectInputFetcher::createDikKeyMap()
{
	m_dikFromKeyMap[InputHelper::W]		= DIK_W;
	m_dikFromKeyMap[InputHelper::A]		= DIK_A;
	m_dikFromKeyMap[InputHelper::S]		= DIK_S;
	m_dikFromKeyMap[InputHelper::D]		= DIK_D;
	m_dikFromKeyMap[InputHelper::L]		= DIK_L;
	m_dikFromKeyMap[InputHelper::SPACE]	= DIK_SPACE;
	m_dikFromKeyMap[InputHelper::LCTRL]	= DIK_LCONTROL;
	m_dikFromKeyMap[InputHelper::ESC]	= DIK_ESCAPE;
	m_dikFromKeyMap[InputHelper::F1]	= DIK_F1;
	m_dikFromKeyMap[InputHelper::F2]	= DIK_F2;
	m_dikFromKeyMap[InputHelper::F3]	= DIK_F3;
	m_dikFromKeyMap[InputHelper::F4]	= DIK_F4;
}

void DirectInputFetcher::reset()
{
	for( int i=0; i<InputHelper::NUM_MOUSE_BTNS; i++)
		m_mouseBtns[i] = InputHelper::KEY_UP;

	for( int i=0; i<InputHelper::NUM_KEYBOARD_KEYS; i++)
		m_mouseBtns[i] = InputHelper::KEY_UP;

	for( int i=0; i<InputHelper::NUM_MOUSE_AXIS+1; i++ )
	{
		m_mousePos[i] = 0;
		m_mouseTravel[i] = 0;
	}
}

void DirectInputFetcher::detectInput(void)
{
	// Get access if we don't have it already
	m_dinkeyboard->Acquire();
	m_dinmouse->Acquire();

	// Get the input data
	m_dinkeyboard->GetDeviceState(256, (LPVOID)m_keystate);
	m_dinmouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mousestate);

	// Bogus data is returned when the window lost focus. Do not let that corrupt the fine
	// input data
	if(m_mousestate.rgbButtons[0] == 0xcdcdcdcd &&
		m_keystate[0] == 205)
	{
		for( int i=0; i<NUM_KEYS; i++)
		{
			m_keystate[i] = '\0';
		}

		for( int i=0; i<4; i++)
		{
			m_mousestate.rgbButtons[i] = 0;
		}
		m_mousestate.lX = 0;
		m_mousestate.lY = 0;
		m_mousestate.lZ = 0;
	}
}

#include "DirectInputFetcher.h"
#include "DirectInputException.h"

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
	m_mouseTravel[InputHelper::X_POSITIVE] = (int)m_mousestate.lX;
	m_mouseTravel[InputHelper::Y_POSITIVE] = (int)m_mousestate.lY;
	m_mouseTravel[InputHelper::Z_POSITIVE] = (int)m_mousestate.lZ;
	m_mouseTravel[InputHelper::X_NEGATIVE] = (int)m_mousestate.lX;
	m_mouseTravel[InputHelper::Y_NEGATIVE] = (int)m_mousestate.lY;
	m_mouseTravel[InputHelper::Z_NEGATIVE] = (int)m_mousestate.lZ;

	//HACK: Could be done by using mouseTravel:
	// The mouse position is being fetched not through Direct InputHelper but through
	// the windows api. The z-component (scroll wheel) is fetched through DI.
	POINT cursorPos;
	GetCursorPos(&cursorPos);
	m_mousePos[InputHelper::X_POSITIVE] = (int)cursorPos.x;
	m_mousePos[InputHelper::Y_POSITIVE] = (int)cursorPos.y;
	m_mousePos[InputHelper::Z_POSITIVE] += (int)m_mousestate.lZ;
	m_mousePos[InputHelper::X_NEGATIVE] = (int)cursorPos.x;
	m_mousePos[InputHelper::Y_NEGATIVE] = (int)cursorPos.y;
	m_mousePos[InputHelper::Z_NEGATIVE] += (int)m_mousestate.lZ;

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
	hr = DirectInput8Create( m_hInstance,    // the handle to the application
		DIRECTINPUT_VERSION,    // the compatible version
		IID_IDirectInput8,    // the DirectInput interface version
		(void**)&m_din,    // the pointer to the interface
		NULL);				// COM stuff, so we'll set it to NULL
	checkDirectInputHr( hr, __FILE__, __FUNCTION__, __LINE__ );

	hr = m_din->CreateDevice( GUID_SysKeyboard,    // the default keyboard ID being used
		&m_dinkeyboard,    // the pointer to the device interface
		NULL);		// COM stuff, so we'll set it to NULL
	checkDirectInputHr( hr, __FILE__, __FUNCTION__, __LINE__ );

	hr = m_din->CreateDevice( GUID_SysMouse,
		&m_dinmouse,
		NULL);
	checkDirectInputHr( hr, __FILE__, __FUNCTION__, __LINE__ );

	// set the data format to keyboard format
	m_dinkeyboard->SetDataFormat(&c_dfDIKeyboard);
	m_dinmouse->SetDataFormat(&c_dfDIMouse);
}

void DirectInputFetcher::acquireDevices()
{
	HRESULT hr = S_OK;
	
	// Get access if we don't have it already
	hr = m_dinkeyboard->Acquire();
	checkDirectInputHr( hr, __FILE__, __FUNCTION__, __LINE__ );

	hr = m_dinmouse->Acquire();
	checkDirectInputHr( hr, __FILE__, __FUNCTION__, __LINE__ );
}

void DirectInputFetcher::checkDirectInputHr( HRESULT p_hr, const string &p_file,
											const string& p_func, int p_line )
{
	if( FAILED(p_hr) )
	{
		if( !m_onlyForeground && p_hr == E_ACCESSDENIED )
		{
			throw DirectInputException( p_hr, p_file, p_func, p_line );
		}
	}
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
	m_dikFromKeyMap[InputHelper::KEY_A]		= DIK_A;
	m_dikFromKeyMap[InputHelper::KEY_B]		= DIK_B;
	m_dikFromKeyMap[InputHelper::KEY_C]		= DIK_C;
	m_dikFromKeyMap[InputHelper::KEY_D]		= DIK_D;
	m_dikFromKeyMap[InputHelper::KEY_E]		= DIK_E;
	m_dikFromKeyMap[InputHelper::KEY_F]		= DIK_F;
	m_dikFromKeyMap[InputHelper::KEY_G]		= DIK_G;
	m_dikFromKeyMap[InputHelper::KEY_H]		= DIK_H;
	m_dikFromKeyMap[InputHelper::KEY_I]		= DIK_I;
	m_dikFromKeyMap[InputHelper::KEY_J]		= DIK_J;
	m_dikFromKeyMap[InputHelper::KEY_K]		= DIK_K;
	m_dikFromKeyMap[InputHelper::KEY_L]		= DIK_L;
	m_dikFromKeyMap[InputHelper::KEY_M]		= DIK_M;
	m_dikFromKeyMap[InputHelper::KEY_N]		= DIK_N;
	m_dikFromKeyMap[InputHelper::KEY_O]		= DIK_O;
	m_dikFromKeyMap[InputHelper::KEY_P]		= DIK_P;
	m_dikFromKeyMap[InputHelper::KEY_Q]		= DIK_Q;
	m_dikFromKeyMap[InputHelper::KEY_R]		= DIK_R;
	m_dikFromKeyMap[InputHelper::KEY_S]		= DIK_S;
	m_dikFromKeyMap[InputHelper::KEY_T]		= DIK_T;
	m_dikFromKeyMap[InputHelper::KEY_U]		= DIK_U;
	m_dikFromKeyMap[InputHelper::KEY_V]		= DIK_V;
	m_dikFromKeyMap[InputHelper::KEY_W]		= DIK_W;
	m_dikFromKeyMap[InputHelper::KEY_X]		= DIK_X;
	m_dikFromKeyMap[InputHelper::KEY_Y]		= DIK_Y;
	m_dikFromKeyMap[InputHelper::KEY_Z]		= DIK_Z;

	m_dikFromKeyMap[InputHelper::KEY_SPACE]	= DIK_SPACE;
	m_dikFromKeyMap[InputHelper::KEY_LCTRL]	= DIK_LCONTROL;
	m_dikFromKeyMap[InputHelper::KEY_ESC]	= DIK_ESCAPE;
	m_dikFromKeyMap[InputHelper::KEY_F1]	= DIK_F1;
	m_dikFromKeyMap[InputHelper::KEY_F2]	= DIK_F2;
	m_dikFromKeyMap[InputHelper::KEY_F3]	= DIK_F3;
	m_dikFromKeyMap[InputHelper::KEY_F4]	= DIK_F4;

	m_dikFromKeyMap[InputHelper::KEY_0]	= DIK_0;
	m_dikFromKeyMap[InputHelper::KEY_1]	= DIK_1;
	m_dikFromKeyMap[InputHelper::KEY_2]	= DIK_2;
	m_dikFromKeyMap[InputHelper::KEY_3]	= DIK_3;
	m_dikFromKeyMap[InputHelper::KEY_4]	= DIK_4;
	m_dikFromKeyMap[InputHelper::KEY_5]	= DIK_5;
	m_dikFromKeyMap[InputHelper::KEY_6]	= DIK_6;
	m_dikFromKeyMap[InputHelper::KEY_7]	= DIK_7;
	m_dikFromKeyMap[InputHelper::KEY_8]	= DIK_8;
	m_dikFromKeyMap[InputHelper::KEY_9]	= DIK_9;

	m_dikFromKeyMap[InputHelper::KEY_NUM0]	= DIK_NUMPAD0;
	m_dikFromKeyMap[InputHelper::KEY_NUM1]	= DIK_NUMPAD1;
	m_dikFromKeyMap[InputHelper::KEY_NUM2]	= DIK_NUMPAD2;
	m_dikFromKeyMap[InputHelper::KEY_NUM3]	= DIK_NUMPAD3;
	m_dikFromKeyMap[InputHelper::KEY_NUM4]	= DIK_NUMPAD4;
	m_dikFromKeyMap[InputHelper::KEY_NUM5]	= DIK_NUMPAD5;
	m_dikFromKeyMap[InputHelper::KEY_NUM6]	= DIK_NUMPAD6;
	m_dikFromKeyMap[InputHelper::KEY_NUM7]	= DIK_NUMPAD7;
	m_dikFromKeyMap[InputHelper::KEY_NUM8]	= DIK_NUMPAD8;
	m_dikFromKeyMap[InputHelper::KEY_NUM9]	= DIK_NUMPAD9;
}

void DirectInputFetcher::reset()
{
	for( int i=0; i<InputHelper::NUM_MOUSE_BTNS; i++)
	{
		m_mouseBtns[i] = InputHelper::KEY_UP;
	}

	for( int i=0; i<InputHelper::NUM_KEYBOARD_KEYS; i++)
	{
		m_kbKeys[i] = InputHelper::KEY_UP;
	}

	for( int i=0; i<InputHelper::NUM_MOUSE_AXIS; i++ )
	{
		m_mousePos[i] = 0;
		m_mouseTravel[i] = 0;
	}
}

void DirectInputFetcher::detectInput(void)
{
	// Get the input data
	if( m_dinkeyboard->GetDeviceState(256, (LPVOID)m_keystate) != DI_OK )
	{
		acquireDevices();
		for( int i=0; i<NUM_KEYS; i++)
		{
			m_keystate[i] = '\0';
		}
	}
	
	if( m_dinmouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mousestate) != DI_OK )
	{
		acquireDevices();
		for( int i=0; i<4; i++)
		{
			m_mousestate.rgbButtons[i] = 0;
		}
		m_mousestate.lX = 0;
		m_mousestate.lY = 0;
		m_mousestate.lZ = 0;
	}
	else
	{
		int breakHere = 0;
	}
}
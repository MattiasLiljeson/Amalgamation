#include "DirectInputFetcher.h"
#include "DirectInputException.h"
#include "..\..\Util\Src\DebugUtil.h"

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
	m_mouseSensitivity = 1.0f;

	if( m_hWnd != 0 && m_hInstance != 0 )
	{
		init();
	}
	else
	{
		// Throw exception, use a describing string instead of function as no good HR can
		// be given. The user can still find the faulty line with the file name and line
		// number provided.
		throw DirectInputException( S_OK, __FILE__, "HINSTANCE or HWND not specified!", __LINE__ );
		int breakHere = 0;
	}
}

DirectInputFetcher::~DirectInputFetcher()
{
	m_dinkeyboard->Unacquire();		// make sure the keyboard is unacquired
	m_dinmouse->Unacquire();		// make sure the mouse in unacquired
	m_din->Release();				// close DirectInput before exiting
}

void DirectInputFetcher::update()
{
	detectInput();

	// Mouse travel (delta position / movement)
	m_mouseTravel[InputHelper::MouseAxes_X_POSITIVE] = (int)m_mousestate.lX;
	m_mouseTravel[InputHelper::MouseAxes_Y_POSITIVE] = (int)m_mousestate.lY;
	m_mouseTravel[InputHelper::MouseAxes_Z_POSITIVE] = (int)m_mousestate.lZ;
	m_mouseTravel[InputHelper::MouseAxes_X_NEGATIVE] = (int)m_mousestate.lX;
	m_mouseTravel[InputHelper::MouseAxes_Y_NEGATIVE] = (int)m_mousestate.lY;
	m_mouseTravel[InputHelper::MouseAxes_Z_NEGATIVE] = (int)m_mousestate.lZ;

	//NOTE: Could be done by using mouseTravel:
	// The mouse position is being fetched not through Direct InputHelper but through
	// the windows api. The z-component (scroll wheel) is fetched through DI.
	POINT cursorPos;
	GetCursorPos(&cursorPos);
	m_mousePos[InputHelper::MouseAxes_X_POSITIVE] = (int)cursorPos.x;
	m_mousePos[InputHelper::MouseAxes_Y_POSITIVE] = (int)cursorPos.y;
	m_mousePos[InputHelper::MouseAxes_Z_POSITIVE] += (int)m_mousestate.lZ;
	m_mousePos[InputHelper::MouseAxes_X_NEGATIVE] = (int)cursorPos.x;
	m_mousePos[InputHelper::MouseAxes_Y_NEGATIVE] = (int)cursorPos.y;
	m_mousePos[InputHelper::MouseAxes_Z_NEGATIVE] += (int)m_mousestate.lZ;

	for( int i=0; i<InputHelper::MouseButtons_CNT; i++)
	{
		m_mouseBtns[i] = InputHelper::calcState( m_mouseBtns[i], (bool)m_mousestate.rgbButtons[i] );
	}
	for( int i=0; i<InputHelper::KeyboardKeys_CNT; i++)
	{
		m_kbKeys[i] = InputHelper::calcState( m_kbKeys[i], (m_keystate[m_dikFromKeyMap[i]] & 0x80) );
	}
}

InputHelper::KeyStates DirectInputFetcher::getKeyState( int p_key )
{
	if( 0 <= p_key && p_key < InputHelper::KeyboardKeys_CNT )
		return m_kbKeys[p_key];
	else
		return InputHelper::KeyStates_KEY_UP;
}

InputHelper::KeyStates DirectInputFetcher::getMouseBtnState( int p_key )
{
	if( 0 <= p_key && p_key < InputHelper::MouseButtons_CNT )
		return m_mouseBtns[p_key];
	else
		return InputHelper::KeyStates_KEY_UP;
}

int DirectInputFetcher::getMousePos(int axis)
{
	if( 0 <= axis && axis < InputHelper::MouseAxes_CNT )
		return m_mousePos[axis];
	else
		return 0;
}

int DirectInputFetcher::getMouseTravel(int axis)
{
	if( 0 <= axis && axis < InputHelper::MouseAxes_CNT )
		return m_mouseTravel[axis];
	else
		return 0;
}

void DirectInputFetcher::setMouseSensitivity( const double p_sensitivity )
{
	m_mouseSensitivity = p_sensitivity;
}

const double DirectInputFetcher::getMouseSensitivity() const
{
	return m_mouseSensitivity;
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
	m_dikFromKeyMap[InputHelper::KeyboardKeys_A]			= DIK_A;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_B]			= DIK_B;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_C]			= DIK_C;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_D]			= DIK_D;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_E]			= DIK_E;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_F]			= DIK_F;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_G]			= DIK_G;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_H]			= DIK_H;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_I]			= DIK_I;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_J]			= DIK_J;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_K]			= DIK_K;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_L]			= DIK_L;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_M]			= DIK_M;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_N]			= DIK_N;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_O]			= DIK_O;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_P]			= DIK_P;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_Q]			= DIK_Q;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_R]			= DIK_R;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_S]			= DIK_S;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_T]			= DIK_T;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_U]			= DIK_U;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_V]			= DIK_V;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_W]			= DIK_W;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_X]			= DIK_X;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_Y]			= DIK_Y;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_Z]			= DIK_Z;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_AA]			= DIK_LBRACKET;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_AE]			= DIK_APOSTROPHE;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_OE]			= DIK_SEMICOLON;

	m_dikFromKeyMap[InputHelper::KeyboardKeys_LEFT_SHIFT]	= DIK_LSHIFT;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_LEFT_CTRL]	= DIK_LCONTROL;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_LEFT_ALT]		= DIK_LMENU;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_SPACE]		= DIK_SPACE;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_RIGHT_ALT]	= DIK_RMENU;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_RIGHT_CTRL]	= DIK_RCONTROL;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_RIGHT_SHIFT]	= DIK_RSHIFT;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_BACKSPACE]	= DIK_BACKSPACE;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_DELETE]		= DIK_DELETE;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_RETURN]		= DIK_RETURN;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_PERIOD]		= DIK_PERIOD;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_ESC]			= DIK_ESCAPE;

	m_dikFromKeyMap[InputHelper::KeyboardKeys_TAB]			= DIK_TAB;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_CAPS_LOCK]	= DIK_CAPITAL;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_NUM_LOCK]		= DIK_NUMLOCK;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_SCROLL_LOCK]	= DIK_SCROLL;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_LEFT_WIN]		= DIK_LWIN;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_RIGHT_WIN]	= DIK_RWIN;

	m_dikFromKeyMap[InputHelper::KeyboardKeys_LEFT]			= DIK_LEFT;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_UP]			= DIK_UP;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_DOWN]			= DIK_DOWN;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_RIGHT]		= DIK_RIGHT;

	m_dikFromKeyMap[InputHelper::KeyboardKeys_LEFT_ARROW]	= DIK_LEFTARROW;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_UP_ARROW]		= DIK_UPARROW;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_DOWN_ARROW]	= DIK_DOWNARROW;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_RIGHT_ARROW]	= DIK_RIGHTARROW;

	m_dikFromKeyMap[InputHelper::KeyboardKeys_F1]			= DIK_F1;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_F2]			= DIK_F2;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_F3]			= DIK_F3;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_F4]			= DIK_F4;

	m_dikFromKeyMap[InputHelper::KeyboardKeys_0]			= DIK_0;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_1]			= DIK_1;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_2]			= DIK_2;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_3]			= DIK_3;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_4]			= DIK_4;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_5]			= DIK_5;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_6]			= DIK_6;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_7]			= DIK_7;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_8]			= DIK_8;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_9]			= DIK_9;

	m_dikFromKeyMap[InputHelper::KeyboardKeys_NUMPAD_0]		= DIK_NUMPAD0;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_NUMPAD_1]		= DIK_NUMPAD1;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_NUMPAD_2]		= DIK_NUMPAD2;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_NUMPAD_3]		= DIK_NUMPAD3;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_NUMPAD_4]		= DIK_NUMPAD4;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_NUMPAD_5]		= DIK_NUMPAD5;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_NUMPAD_6]		= DIK_NUMPAD6;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_NUMPAD_7]		= DIK_NUMPAD7;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_NUMPAD_8]		= DIK_NUMPAD8;
	m_dikFromKeyMap[InputHelper::KeyboardKeys_NUMPAD_9]		= DIK_NUMPAD9;
}

void DirectInputFetcher::reset()
{
	for( int i=0; i<InputHelper::MouseButtons_CNT; i++)
	{
		m_mouseBtns[i] = InputHelper::KeyStates_KEY_UP;
	}

	for( int i=0; i<InputHelper::KeyboardKeys_CNT; i++)
	{
		m_kbKeys[i] = InputHelper::KeyStates_KEY_UP;
	}

	for( int i=0; i<InputHelper::MouseAxes_CNT; i++ )
	{
		m_mousePos[i] = 0;
		m_mouseTravel[i] = 0;
	}
}

void DirectInputFetcher::detectInput(void)
{
	// Get the input data
	int keyboardDetectStatus = m_dinkeyboard->GetDeviceState( 256, (LPVOID)m_keystate );
	if( keyboardDetectStatus != DI_OK )
	{
		acquireDevices();
		for( int i=0; i<NUM_KEYS; i++ )
		{
			m_keystate[i] = '\0';
		}
	}
	
	int mouseDetectStatus = m_dinmouse->GetDeviceState( sizeof(DIMOUSESTATE), (LPVOID)&m_mousestate );
	if( mouseDetectStatus != DI_OK )
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

void DirectInputFetcher::setMouseCooperation( bool p_exclusive )
{
	DWORD settings = DISCL_FOREGROUND;
	m_dinmouse->Unacquire();
	reset();
	if( p_exclusive )
	{
		settings |= DISCL_EXCLUSIVE;
		DEBUGPRINT(("\n\nNow exclusive\n\n"));
	}
	else
	{
		settings |= DISCL_NONEXCLUSIVE;
		DEBUGPRINT(("\n\nNow non-exclusive\n\n"));
	}

	m_dinmouse->SetCooperativeLevel( m_hWnd, settings );
	acquireDevices();
}

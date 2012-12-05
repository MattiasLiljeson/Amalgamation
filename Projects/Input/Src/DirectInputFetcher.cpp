#include "DirectInputFetcher.h"

DirectInputFetcher::DirectInputFetcher(HINSTANCE* _hInstance, HWND* _hWnd)
{
	hInstance = _hInstance;
	hWnd = _hWnd;

	// create the DirectInput interface
	DirectInput8Create(*hInstance,    // the handle to the application
					   DIRECTINPUT_VERSION,    // the compatible version
					   IID_IDirectInput8,    // the DirectInput interface version
					   (void**)&din,    // the pointer to the interface
					   NULL);    // COM stuff, so we'll set it to NULL

	// create the keyboard device
	din->CreateDevice(GUID_SysKeyboard,    // the default keyboard ID being used
					  &dinkeyboard,    // the pointer to the device interface
					  NULL);    // COM stuff, so we'll set it to NULL
	din->CreateDevice(GUID_SysMouse,
					  &dinmouse,
					  NULL);

	// set the data format to keyboard format
	dinkeyboard->SetDataFormat(&c_dfDIKeyboard);
	dinmouse->SetDataFormat(&c_dfDIMouse);

	// set the control we will have over the keyboard
	dinkeyboard->SetCooperativeLevel(*hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	dinmouse->SetCooperativeLevel(*hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

	reset();
	m_dikFromKeyMap[InputHelper::W]		= DIK_W;
	m_dikFromKeyMap[InputHelper::A]		= DIK_A;
	m_dikFromKeyMap[InputHelper::S]		= DIK_S;
	m_dikFromKeyMap[InputHelper::D]		= DIK_D;
	m_dikFromKeyMap[InputHelper::SPACE]	= DIK_SPACE;
	m_dikFromKeyMap[InputHelper::LCTRL]	= DIK_LCONTROL;
	m_dikFromKeyMap[InputHelper::ESC]		= DIK_ESCAPE;
	m_dikFromKeyMap[InputHelper::F1]		= DIK_F1;
	m_dikFromKeyMap[InputHelper::F2]		= DIK_F2;
	m_dikFromKeyMap[InputHelper::F3]		= DIK_F3;
	m_dikFromKeyMap[InputHelper::F4]		= DIK_F4;
}

DirectInputFetcher::~DirectInputFetcher()
{
	dinkeyboard->Unacquire();    // make sure the keyboard is unacquired
	dinmouse->Unacquire();    // make sure the mouse in unacquired
	din->Release();    // close DirectInput before exiting
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
	dinkeyboard->Acquire();
	dinmouse->Acquire();

	// Get the input data
	dinkeyboard->GetDeviceState(256, (LPVOID)keystate);
	dinmouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mousestate);
}

void DirectInputFetcher::update()
{
	detectInput();

	// Mouse travel (delta position / movement)
	m_mouseTravel[InputHelper::X] = (int)mousestate.lX;
	m_mouseTravel[InputHelper::Y] = (int)mousestate.lY;
	m_mouseTravel[InputHelper::Z] = (int)mousestate.lZ;

	// The mouse position is being fetched not through Direct InputHelper but through
	// the windows api. The z-component (scrollwheel) is fetched thru DI.
	POINT cursorPos;
	GetCursorPos(&cursorPos);
	m_mousePos[InputHelper::X] = (int)cursorPos.x;
	m_mousePos[InputHelper::Y] = (int)cursorPos.y;
	m_mousePos[InputHelper::Z] += (int)mousestate.lZ;

	for( int i=0; i<InputHelper::NUM_MOUSE_BTNS; i++)
		m_mouseBtns[i] = InputHelper::calcState( m_mouseBtns[i], mousestate.rgbButtons[i] );

	for( int i=0; i<InputHelper::NUM_KEYBOARD_KEYS; i++)
		m_kbKeys[i] = InputHelper::calcState( m_kbKeys[i], (keystate[m_dikFromKeyMap[i]] & 0x80) );
}

int DirectInputFetcher::getKeyState( int p_key )
{
	if( 0 <= p_key && p_key < InputHelper::NUM_KEYBOARD_KEYS )
		return m_kbKeys[p_key];
	else
		return InputHelper::KEY_UP;
}

int DirectInputFetcher::getMouseKeyState( int p_key )
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
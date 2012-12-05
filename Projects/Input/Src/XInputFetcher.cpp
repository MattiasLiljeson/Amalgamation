#include "XInputFetcher.h"

int XInputFetcher::s_btnMaskMap[InputHelper::NUM_XBOX360_CONTROLLER_DIGITALS] = 
{
	XINPUT_GAMEPAD_DPAD_UP,
	XINPUT_GAMEPAD_DPAD_DOWN,
	XINPUT_GAMEPAD_DPAD_LEFT, 
	XINPUT_GAMEPAD_DPAD_RIGHT,
	XINPUT_GAMEPAD_START,
	XINPUT_GAMEPAD_BACK,
	XINPUT_GAMEPAD_LEFT_THUMB,
	XINPUT_GAMEPAD_RIGHT_THUMB,
	XINPUT_GAMEPAD_LEFT_SHOULDER,
	XINPUT_GAMEPAD_RIGHT_SHOULDER,
	XINPUT_GAMEPAD_A,
	XINPUT_GAMEPAD_B,
	XINPUT_GAMEPAD_X,
	XINPUT_GAMEPAD_Y
};

XInputFetcher::XInputFetcher()
{
	DWORD dwResult; 
	XINPUT_STATE state;
	ZeroMemory( &state, sizeof(XINPUT_STATE) );

	// Simply get the state of the controller from XInput.
	dwResult = XInputGetState( 0, &state );

	if( dwResult == ERROR_SUCCESS )
	{
		int korv = 1;
		// Controller is connected 
	}
	else
	{
		int korv = 0;
		// Controller is not connected 
	}
}

XInputFetcher::~XInputFetcher()
{
}

void XInputFetcher::update()
{
	XINPUT_STATE newState;
	XInputGetState( 0, &newState );
	for( int i=0; i<InputHelper::NUM_XBOX360_CONTROLLER_DIGITALS; i++)
		m_btns[i] = InputHelper::calcState( m_btns[i], (newState.Gamepad.wButtons & s_btnMaskMap[i]));
	
	m_analogs[InputHelper::THUMB_LX] = m_currentState.Gamepad.sThumbLX;
	m_analogs[InputHelper::THUMB_LY] = m_currentState.Gamepad.sThumbLY;
	m_analogs[InputHelper::THUMB_RX] = m_currentState.Gamepad.sThumbRX;
	m_analogs[InputHelper::THUMB_RY] = m_currentState.Gamepad.sThumbRY;
	m_analogs[InputHelper::TRIGGER_L] = (short)m_currentState.Gamepad.bLeftTrigger;
	m_analogs[InputHelper::TRIGGER_R] = (short)m_currentState.Gamepad.bRightTrigger;
	m_currentState = newState;
}

int XInputFetcher::getBtnState( int p_btn )
{
	if( 0 <= p_btn && p_btn < InputHelper::NUM_XBOX360_CONTROLLER_DIGITALS )
		return m_btns[p_btn];
	else
		return -1;
}

short XInputFetcher::getAnalog( int p_analog )
{
	if( 0 <= p_analog && p_analog < InputHelper::NUM_XBOX360_CONTROLLER_ANALOGS )
		return m_analogs[p_analog];
	else
		return -1;
}

double XInputFetcher::getAnalogAsDouble( int p_analog )
{
	// Std-val. Whats returned if the analog isn't found.
	double val = (float)-1;

	// Make sure that p_analog is in range
	if( 0 <= p_analog && p_analog < InputHelper::NUM_XBOX360_CONTROLLER_ANALOGS )
	{
		short tmp = getAnalog( p_analog );

		// Triggers have a precision of 0-255 instead of 0-65k as the thumb sticks.
		// Take that into account when calculating 0.0 - 1.0 
		if(p_analog == InputHelper::TRIGGER_L || p_analog == InputHelper::TRIGGER_R)
			val =  (double)tmp / 255; //BYTE_MAX
		else
			val = (double)tmp / SHRT_MAX;
	}
	
	return val;
}

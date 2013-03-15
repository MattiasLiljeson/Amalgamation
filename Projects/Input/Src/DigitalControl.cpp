#include "DigitalControl.h"
#include "InputManager.h"
#include "XInputFetcher.h"
#include "InputManager.h"
#include "MessageLoopFetcher.h"

DigitalControl::DigitalControl( InputHelper::Xbox360Digitals p_btn )
	: Control( InputHelper::InputDeviceTypes_XINPUT_DIGITAL )

{
	m_btn = p_btn;
}


DigitalControl::~DigitalControl()
{
}

void DigitalControl::update( float p_dt, InputManager* p_manager )
{
	XInputFetcher* fetcher = p_manager->getXInputFetcher();
	if( fetcher != NULL )
	{
		InputHelper::KeyStates state = fetcher->getBtnState( m_btn );

		m_status = InputHelper::statusFromState( state );
		m_statusDelta = InputHelper::statusDeltaFromState( state );
		m_rawData = state;
	}
}

int DigitalControl::getControlEnum()
{
	return m_btn;
}

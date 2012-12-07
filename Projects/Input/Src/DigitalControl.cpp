#include "DigitalControl.h"


DigitalControl::DigitalControl( InputHelper::XBOX360_CONTROLLER_DIGITAL p_btn )
{
	m_btn = p_btn;
}


DigitalControl::~DigitalControl()
{
}

void DigitalControl::update( InputManager* p_manager )
{
	XInputFetcher* fetcher = p_manager->getXInputFetcher();
	InputHelper::KEY_STATE state = fetcher->getBtnState( m_btn );

	m_status = InputHelper::statusFromState( state );
	m_statusDelta = InputHelper::statusDeltaFromState( state );
	m_rawData = state;
}

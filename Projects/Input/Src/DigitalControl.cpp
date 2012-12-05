#include "DigitalControl.h"


DigitalControl::DigitalControl( InputHelper::XBOX360_CONTROLLER_DIGITALS p_btn )
{
	m_btn = p_btn;
}


DigitalControl::~DigitalControl()
{
}

void DigitalControl::update( InputManager* p_manager )
{
	XInputFetcher* fetcher = p_manager->getXInputFetcher();
	int state = fetcher->getBtnState( m_btn );

	m_status = InputHelper::statusFromState( state );
	m_statusDelta = InputHelper::statusDeltaFromState( state );
	m_rawData = state;
}

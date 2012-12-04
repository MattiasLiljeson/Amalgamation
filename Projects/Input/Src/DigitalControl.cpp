#include "DigitalControl.h"


DigitalControl::DigitalControl( Input::XBOX360_CONTROLLER_DIGITALS p_btn )
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

	m_status = Input::statusFromState( state );
	m_statusDelta = Input::statusDeltaFromState( state );
}

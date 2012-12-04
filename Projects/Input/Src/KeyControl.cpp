#include "KeyControl.h"


KeyControl::KeyControl( Input::KEYBOARD_KEYS p_key )
{
	m_key = p_key;
}

KeyControl::~KeyControl(void)
{
}

void KeyControl::update( InputManager* p_manager )
{
	MessageLoopFetcher* fetcher = p_manager->getMessageLoopFetcher();
	int state = fetcher->getKeyState( m_key );

	m_status = Input::statusFromState( state );
	m_statusDelta = Input::statusDeltaFromState( state );
}

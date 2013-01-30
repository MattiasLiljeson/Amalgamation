#include "KeyControl.h"
#include "InputManager.h"
#include "IMouseKeyboardFetcher.h"


KeyControl::KeyControl( InputHelper::KeyboardKeys p_key )
{
	m_key = p_key;
}

KeyControl::~KeyControl(void)
{
}

void KeyControl::update( InputManager* p_manager )
{
	IMouseKeyboardFetcher* fetcher = p_manager->getMouseKeyboardFetcher();
	InputHelper::KeyStates state = fetcher->getKeyState( m_key );

	m_status = InputHelper::statusFromState( state );
	m_statusDelta = InputHelper::statusDeltaFromState( state );
	m_rawData = state;
}

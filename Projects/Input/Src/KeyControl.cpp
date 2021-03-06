#include "KeyControl.h"
#include "InputManager.h"
#include "IMouseKeyboardFetcher.h"


KeyControl::KeyControl( InputHelper::KeyboardKeys p_key )
	: Control( InputHelper::InputDeviceTypes_KEYBOARD )

{
	m_key = p_key;
}

KeyControl::~KeyControl(void)
{
}

void KeyControl::update( float p_dt, InputManager* p_manager )
{
	IMouseKeyboardFetcher* fetcher = p_manager->getMouseKeyboardFetcher();
	InputHelper::KeyStates state = fetcher->getKeyState( m_key );

	m_status = InputHelper::statusFromState( state );
	m_statusDelta = InputHelper::statusDeltaFromState( state );
	m_rawData = state;
}

int KeyControl::getControlEnum()
{
	return m_key;
}

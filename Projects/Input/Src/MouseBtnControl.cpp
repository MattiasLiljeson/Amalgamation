#include "MouseBtnControl.h"
#include "InputManager.h"
#include "MessageLoopFetcher.h"


MouseBtnControl::MouseBtnControl( InputHelper::MouseButtons p_btn )
	: Control( InputHelper::InputDeviceTypes_MOUSE_BUTTON )

{
	m_btn = p_btn;
}


MouseBtnControl::~MouseBtnControl()
{
}

void MouseBtnControl::update( float p_dt, InputManager* p_manager )
{
	IMouseKeyboardFetcher* fetcher = p_manager->getMouseKeyboardFetcher();
	InputHelper::KeyStates state = fetcher->getMouseBtnState( m_btn );

	m_status = InputHelper::statusFromState( state );
	m_statusDelta = InputHelper::statusDeltaFromState( state );
	m_rawData = state;
}

int MouseBtnControl::getControlEnum()
{
	return m_btn;
}

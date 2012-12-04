#include "MouseBtnControl.h"


MouseBtnControl::MouseBtnControl( Input::MOUSE_BTNS p_btn )
{
	m_btn = p_btn;
}


MouseBtnControl::~MouseBtnControl()
{
}

void MouseBtnControl::update( InputManager* p_manager )
{
	MessageLoopFetcher* fetcher = p_manager->getMessageLoopFetcher();
	int state = fetcher->getMouseBtnState( m_btn );

	m_status = Input::statusFromState( state );
	m_statusDelta = Input::statusDeltaFromState( state );
}

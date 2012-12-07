#include "MouseBtnControl.h"


MouseBtnControl::MouseBtnControl( InputHelper::MOUSE_BTN p_btn )
{
	m_btn = p_btn;
}


MouseBtnControl::~MouseBtnControl()
{
}

void MouseBtnControl::update( InputManager* p_manager )
{
	MessageLoopFetcher* fetcher = p_manager->getMessageLoopFetcher();
	InputHelper::KEY_STATE state = fetcher->getMouseBtnState( m_btn );

	m_status = InputHelper::statusFromState( state );
	m_statusDelta = InputHelper::statusDeltaFromState( state );
	m_rawData = state;
}
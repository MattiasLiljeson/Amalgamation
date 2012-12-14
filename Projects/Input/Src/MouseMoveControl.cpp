#include "MouseMoveControl.h"


MouseMoveControl::MouseMoveControl( InputHelper::MOUSE_AXIS p_axis, InputHelper::SUB_AXIS p_subAxis )
{
	m_axis = p_axis;
	m_subAxis = p_subAxis;
}


MouseMoveControl::~MouseMoveControl()
{
}

void MouseMoveControl::update( InputManager* p_manager )
{
	IMouseKeyboardFetcher* fetcher = p_manager->getMouseKeyboardFetcher();
	int travel = fetcher->getMouseTravel( m_axis );
	
	double newStatus = 0.0;
	if( (m_subAxis == InputHelper:: AXIS_POSITIVE && travel > 0) ||
		(m_subAxis == InputHelper:: AXIS_NEGATIVE && travel < 0) )
	{
		travel = abs( travel );
		newStatus = (double)travel / SHRT_MAX;
	}
	
	m_statusDelta = newStatus - m_status;
	m_status = newStatus;
	m_rawData = travel;
}

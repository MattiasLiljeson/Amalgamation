#include "MouseMoveControl.h"


MouseMoveControl::MouseMoveControl( Input::MOUSE_AXIS p_axis, Input::SUB_AXIS p_subAxis )
{
	m_axis = p_axis;
	m_subAxis = p_subAxis;
}


MouseMoveControl::~MouseMoveControl()
{
}

void MouseMoveControl::update( InputManager* p_manager )
{
	MessageLoopFetcher* fetcher = p_manager->getMessageLoopFetcher();
	int travel = fetcher->getMouseTravel( m_axis );
	
	double newStatus = 0.0;
	if( (m_subAxis == Input:: AXIS_POSITIVE && travel > 0) ||
		(m_subAxis == Input:: AXIS_NEGATIVE && travel < 0) )
	{
		travel = abs( travel );
		newStatus = (double)travel / SHRT_MAX;
	}
	
	m_statusDelta = newStatus - m_status;
}

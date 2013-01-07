#include "MouseMoveControl.h"
#include "InputManager.h"
#include "MessageLoopFetcher.h"


MouseMoveControl::MouseMoveControl( InputHelper::MOUSE_AXIS p_axis)
{
	m_axis = p_axis;

	//HACK: Multiple fallthroughs!
	switch( p_axis )
	{
	case InputHelper::X_POSITIVE:
	case InputHelper::Y_POSITIVE:
	case InputHelper::Z_POSITIVE:
		m_subAxis = InputHelper::AXIS_POSITIVE;
		break;

	case InputHelper::X_NEGATIVE:
	case InputHelper::Y_NEGATIVE:
	case InputHelper::Z_NEGATIVE:
		m_subAxis = InputHelper::AXIS_NEGATIVE;
		break;
	}
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
	m_rawData = fetcher->getMouseTravel( m_axis );;
}

#include "AnalogueControl.h"


AnalogueControl::AnalogueControl( InputHelper::XBOX360_CONTROLLER_ANALOG p_axis )
{
	m_axis = p_axis;

	//HACK: Multiple fallthroughs!
	switch( p_axis )
	{
	case InputHelper::TRIGGER_L:
	case InputHelper::TRIGGER_R:
	case InputHelper::THUMB_LX_POSITIVE:
	case InputHelper::THUMB_LY_POSITIVE:
	case InputHelper::THUMB_RX_POSITIVE:
	case InputHelper::THUMB_RY_POSITIVE:
		m_subAxis = InputHelper::AXIS_POSITIVE;
		break;

	case InputHelper::THUMB_LX_NEGATIVE:
	case InputHelper::THUMB_LY_NEGATIVE:
	case InputHelper::THUMB_RX_NEGATIVE:
	case InputHelper::THUMB_RY_NEGATIVE:
		m_subAxis = InputHelper::AXIS_NEGATIVE;
		break;

	default:
		//should NEVER happen!
		m_subAxis = InputHelper::AXIS_POSITIVE;
		break;
	}
}

AnalogueControl::~AnalogueControl()
{
}

void AnalogueControl::update( InputManager* p_manager )
{
	XInputFetcher* fetcher = p_manager->getXInputFetcher();
	double travel = fetcher->getAnalogAsDouble( m_axis );

	double newStatus = 0.0;
	if( (m_subAxis == InputHelper:: AXIS_POSITIVE && travel > 0.0) ||
		(m_subAxis == InputHelper:: AXIS_NEGATIVE && travel < 0.0) )
	{
		newStatus = fabs( travel );
	}

	m_rawData = fetcher->getAnalog( m_axis );
	m_statusDelta = newStatus - m_status;
	m_status = newStatus;
}

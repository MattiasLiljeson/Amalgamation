#include "AnalogueControl.h"
#include "InputManager.h"
#include "XInputFetcher.h"


AnalogueControl::AnalogueControl( InputHelper::Xbox360Analogs p_axis )
	: Control( InputHelper::InputDeviceTypes_XINPUT_ANALOG )

{
	m_axis = p_axis;

	//NOTE: Multiple fallthroughs!
	switch( p_axis )
	{
	case InputHelper::Xbox360Analogs_TRIGGER_L:
	case InputHelper::Xbox360Analogs_TRIGGER_R:
	case InputHelper::Xbox360Analogs_THUMB_LX_POSITIVE:
	case InputHelper::Xbox360Analogs_THUMB_LY_POSITIVE:
	case InputHelper::Xbox360Analogs_THUMB_RX_POSITIVE:
	case InputHelper::Xbox360Analogs_THUMB_RY_POSITIVE:
		m_subAxis = InputHelper::SubAxes_POSITIVE;
		break;

	case InputHelper::Xbox360Analogs_THUMB_LX_NEGATIVE:
	case InputHelper::Xbox360Analogs_THUMB_LY_NEGATIVE:
	case InputHelper::Xbox360Analogs_THUMB_RX_NEGATIVE:
	case InputHelper::Xbox360Analogs_THUMB_RY_NEGATIVE:
		m_subAxis = InputHelper::SubAxes_NEGATIVE;
		break;

	default:
		//should NEVER happen!
		m_subAxis = InputHelper::SubAxes_POSITIVE;
		break;
	}
}

AnalogueControl::~AnalogueControl()
{
}

void AnalogueControl::update( float p_dt, InputManager* p_manager )
{
	XInputFetcher* fetcher = p_manager->getXInputFetcher();
	double mouseSensitivity = fetcher->getControllerSensitivity();
	double travel = fetcher->getCalibratedAnalog( m_axis );
	travel *= mouseSensitivity;

	double newStatus = 0.0;
	if( (m_subAxis == InputHelper:: SubAxes_POSITIVE && travel > 0.0) ||
		(m_subAxis == InputHelper:: SubAxes_NEGATIVE && travel < 0.0) )
	{
		newStatus = fabs( travel );
	}

	m_rawData = fetcher->getRawAnalog( m_axis );
	m_statusDelta = newStatus - m_status;
	m_status = newStatus;
}

int AnalogueControl::getControlEnum()
{
	return m_axis;
}

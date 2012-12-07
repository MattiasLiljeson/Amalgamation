#include "AnalogueControl.h"


AnalogueControl::AnalogueControl( InputHelper::XBOX360_CONTROLLER_ANALOG p_axis, InputHelper::SUB_AXIS p_subAxis )
{
	m_axis = p_axis;
	m_subAxis = p_subAxis;
}

AnalogueControl::~AnalogueControl(void)
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

#include "AnalogueControl.h"


AnalogueControl::AnalogueControl( Input::XBOX360_CONTROLLER_ANALOGS p_axis, Input::SUB_AXIS p_subAxis )
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
	if( (m_subAxis == Input:: AXIS_POSITIVE && travel > 0.0) ||
		(m_subAxis == Input:: AXIS_NEGATIVE && travel < 0.0) )
	{
		newStatus = fabs( travel );
	}

	m_statusDelta = newStatus - m_status;
}

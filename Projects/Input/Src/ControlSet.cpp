#include "Control.h"

#include "ControlSet.h"


InputHelper::KEY_STATE ControlSet::getPrimaryState()
{
	return getState( m_primaryBtn );
}

InputHelper::KEY_STATE ControlSet::getSecondaryState()
{
	return getState( m_secondaryBtn );
}

InputHelper::KEY_STATE ControlSet::getState( Control* p_control )
{
	int rawData = p_control->getRawData();
	return static_cast<InputHelper::KEY_STATE>(rawData);
}

double ControlSet::dx()
{
	double x = 0.0;
	x -= m_xNegative->getStatus() * m_xSensitivity;
	x += m_xPositive->getStatus() * m_xSensitivity;
	return x;
}

double ControlSet::dy()
{
	double y = 0.0;
	y -= m_yNegative->getStatus() * m_ySensitivity;
	y += m_yPositive->getStatus() * m_ySensitivity;
	return y;
}

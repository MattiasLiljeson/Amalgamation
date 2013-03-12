#include "Control.h"

#include "ControlSet.h"


InputHelper::KeyStates ControlSet::getPrimaryState()
{
	return getState( m_primaryBtn );
}

InputHelper::KeyStates ControlSet::getSecondaryState()
{
	return getState( m_secondaryBtn );
}

InputHelper::KeyStates ControlSet::getState( Control* p_control )
{
	int rawData = p_control->getRawData();
	return static_cast<InputHelper::KeyStates>(rawData);
}

double ControlSet::dx(double p_screenWidth)
{
	double x = 0.0;
	x -= m_xNegative->getStatus() * m_xSensitivity/p_screenWidth;
	x += m_xPositive->getStatus() * m_xSensitivity/p_screenWidth;
	return x;
}

double ControlSet::dy(double p_screenHeight)
{
	double y = 0.0;
	y -= m_yNegative->getStatus() * m_ySensitivity/p_screenHeight;
	y += m_yPositive->getStatus() * m_ySensitivity/p_screenHeight;
	return y;
}

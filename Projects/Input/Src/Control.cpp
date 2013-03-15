#include "Control.h"

Control::Control( InputHelper::InputDeviceTypes p_deviceType )
{
	m_deviceType = p_deviceType;
	m_status = 0.0;
	m_statusDelta = 0.0;
	m_rawData = 0;
}

Control::~Control()
{
}

double Control::getStatus()
{
	return m_status;
}

double Control::getDelta()
{
	return m_statusDelta;
}

int Control::getRawData()
{
	return m_rawData;
}

double* Control::getStatusPtr()
{
	return &m_status;
}

InputHelper::InputDeviceTypes Control::getDeviceType()
{
	return m_deviceType;
}

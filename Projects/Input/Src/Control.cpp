#include "Control.h"


Control::Control()
{
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

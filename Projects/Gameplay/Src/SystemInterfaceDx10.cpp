#include "SystemInterfaceDx10.h"

SystemInterfaceDx10::SystemInterfaceDx10(Timer* _timer)
{
	timer = _timer;
}

// Get the number of seconds elapsed since the start of the application
float SystemInterfaceDx10::GetElapsedTime()
{
	return timer->getGameTime();
}
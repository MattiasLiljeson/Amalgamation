#ifndef SYSTEMINTERFACEDX10_H
#define SYSTEMINTERFACEDX10_H

#include <Rocket/Core/SystemInterface.h>

#include "Timer.h"

class SystemInterfaceDx10 : public Rocket::Core::SystemInterface
{
private:
	Timer* timer;

public:
	SystemInterfaceDx10(Timer* _timer);

	/// Get the number of seconds elapsed since the start of the application
	/// @returns Seconds elapsed
	virtual float GetElapsedTime();
};

#endif
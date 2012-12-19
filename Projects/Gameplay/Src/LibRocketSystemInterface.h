#ifndef SYSTEMINTERFACEDX10_H
#define SYSTEMINTERFACEDX10_H

#include <Rocket/Core/SystemInterface.h>

#include <EntityWorld.h>

class EntityWorld;

class LibRocketSystemInterface : public Rocket::Core::SystemInterface
{
private:
	EntityWorld* m_world;

public:
	LibRocketSystemInterface( EntityWorld* p_world );

	/// Get the number of seconds elapsed since the start of the application
	/// @returns Seconds elapsed
	virtual float GetElapsedTime();
};

#endif
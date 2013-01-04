#include "LibRocketSystemInterface.h"

#include <EntityWorld.h>

LibRocketSystemInterface::LibRocketSystemInterface( EntityWorld* p_world )
{
	m_world = p_world;
}

// Get the number of seconds elapsed since the start of the application
float LibRocketSystemInterface::GetElapsedTime()
{
	return m_world->getElapsedTime();
}
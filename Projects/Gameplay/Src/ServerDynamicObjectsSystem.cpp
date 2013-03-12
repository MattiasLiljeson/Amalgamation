#include "ServerDynamicObjectsSystem.h"

ServerDynamicObjectsSystem::ServerDynamicObjectsSystem() 
	: EntitySystem( SystemType::ServerDynamicObjectsSystem, 1,
	ComponentType::NetworkSynced)
{
}
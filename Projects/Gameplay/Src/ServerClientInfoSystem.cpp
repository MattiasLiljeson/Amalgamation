#include "ServerClientInfoSystem.h"

ServerClientInfoSystem::ServerClientInfoSystem() 
	: EntitySystem(SystemType::ServerClientInfoSystem, 1, ComponentType::ClientInfo)
{

}

ServerClientInfoSystem::~ServerClientInfoSystem()
{

}


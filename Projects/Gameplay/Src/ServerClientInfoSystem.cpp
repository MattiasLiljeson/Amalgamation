#include "ServerClientInfoSystem.h"
#include "ClientInfo.h"
#include "DebugUtil.h"
#include "TimerSystem.h"

ServerClientInfoSystem::ServerClientInfoSystem() 
	: EntitySystem(SystemType::ServerClientInfoSystem, 1, ComponentType::ClientInfo)
{

}

ServerClientInfoSystem::~ServerClientInfoSystem()
{

}

void ServerClientInfoSystem::inserted( Entity* p_entity )
{
	//auto info = static_cast<ClientInfo*>(
	//	p_entity->getComponent(ComponentType::ClientInfo));

	//if (info)
	//	DEBUGPRINT(("OK: Inserted entity contains a clientinfo component\n"));
	//else
	//	DEBUGPRINT(("BUG: Inserted entity does not contain a clientinfo component\n"));
}

void ServerClientInfoSystem::removed( Entity* p_entity )
{
	//auto info = static_cast<ClientInfo*>(
	//	p_entity->getComponent(ComponentType::ClientInfo));

	//if (info)
	//	DEBUGPRINT(("OK: Removed entity contains a clientinfo component\n"));
	//else
	//	DEBUGPRINT(("BUG: Removed entity does not contain a clientinfo component\n"));
}

void ServerClientInfoSystem::processEntities( const vector<Entity*>& p_entities )
{

}





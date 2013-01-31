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
	auto info = static_cast<ClientInfo*>(
		p_entity->getComponent(ComponentType::ClientInfo));

	if (info)
		DEBUGPRINT(("OK: Inserted entity contains a clientinfo component\n"));
	else
		DEBUGPRINT(("BUG: Inserted entity does not contain a clientinfo component\n"));
}

void ServerClientInfoSystem::removed( Entity* p_entity )
{
	auto info = static_cast<ClientInfo*>(
		p_entity->getComponent(ComponentType::ClientInfo));

	if (info)
		DEBUGPRINT(("OK: Removed entity contains a clientinfo component\n"));
	else
		DEBUGPRINT(("BUG: Removed entity does not contain a clientinfo component\n"));
}

void ServerClientInfoSystem::processEntities( const vector<Entity*>& p_entities )
{
	/*auto timerSys = static_cast<TimerSystem*>(m_world->getSystem(SystemType::TimerSystem));

	if (timerSys->checkTimeInterval(TimerIntervals::EverySecond))
	{
		static int cnt = 0;
		if (p_entities.size() <= 6)
		{
			Entity* e = m_world->createEntity();
			e->addComponent(ComponentType::ClientInfo, new ClientInfo(cnt++));
			m_world->addEntity(e);

			e = m_world->createEntity();
			e->addComponent(ComponentType::ClientInfo, new ClientInfo(cnt++));
			m_world->addEntity(e);
		}

		if (!p_entities.empty())
			m_world->deleteEntity(p_entities[0]);
	}*/
}





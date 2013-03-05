#define FORCE_VS_DBG_OUTPUT

#include "TempModuleSpawner.h"
#include "SpawnPointSystem.h"
#include "TimerSystem.h"
#include "EntityFactory.h"
#include "EntityCreationPacket.h"
#include "ServerStateSystem.h"
#include <TcpServer.h>
#include "NetworkSynced.h"

#include <DebugUtil.h>
#include <ToString.h>

TempModuleSpawner::TempModuleSpawner(TcpServer* p_server)
	: EntitySystem(SystemType::TempModuleSpawner)
{
	m_server = p_server;
}

TempModuleSpawner::~TempModuleSpawner()
{

}

void TempModuleSpawner::process()
{
	auto stateSystem = static_cast<ServerStateSystem*>(
		m_world->getSystem(SystemType::ServerStateSystem));

	if (stateSystem->getCurrentState() == ServerStates::INGAME &&
		m_timerSystem->checkTimeInterval(TimerIntervals::EverySecond))
	{
		//DEBUGPRINT(("Request spawning a module at a random position.\n"));
		if (m_spawnPointSystem->isSpawnPointsReady())
		{
			AglMatrix pos = m_spawnPointSystem->getRandomFreeModuleSpawnPoint();

			if (! (pos == m_spawnPointSystem->invalidSpawnPoint()) )
			//while (! (pos == m_spawnPointSystem->invalidSpawnPoint()) )
			{
				EntityCreationPacket cp;

				int randModule = rand() % (EntityType::EndModule - EntityType::ShipModuleStart - 1);
				randModule += EntityType::ShipModuleStart + 1;

				cp.entityType = randModule;
				cp.scale = AglVector3(1.0f, 1.0f, 1.0f);
				Entity* e = m_factory->entityFromPacket(cp, &pos);
	
				AglVector3 posV = pos.GetTranslation();
				string posAsString = toString(posV.x) + " " + toString(posV.y) + " " + toString(posV.z) + "\n";
	
				cp.translation	= pos.GetTranslation();
				cp.rotation		= pos.GetRotation();
				
				auto netSync = static_cast<NetworkSynced*>(
					e->getComponent(ComponentType::NetworkSynced));
				cp.networkIdentity	= netSync->getNetworkIdentity();
				cp.owner			= netSync->getNetworkOwner();
				cp.playerID			= netSync->getPlayerID();

				DEBUGPRINT(((toString("Module spawned at position ") + posAsString).c_str()));

				//pos = m_spawnPointSystem->getRandomFreeModuleSpawnPoint();
				m_server->broadcastPacket(cp.pack());
			}
			else
				setEnabled(false);
		}
		else
		{
			DEBUGPRINT(("Warning: Spawnpoints aren't ready yet.\n"));
		}
	}
}

void TempModuleSpawner::initialize()
{
	m_spawnPointSystem = static_cast<SpawnPointSystem*>(
		m_world->getSystem(SystemType::SpawnPointSystem));

	m_timerSystem = static_cast<TimerSystem*>(
		m_world->getSystem(SystemType::TimerSystem));

	m_factory = static_cast<EntityFactory*>(
		m_world->getSystem(SystemType::EntityFactory));
}




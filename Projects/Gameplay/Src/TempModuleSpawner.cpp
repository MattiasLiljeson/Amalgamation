#define FORCE_VS_DBG_OUTPUT

#include "TempModuleSpawner.h"
#include "SpawnPointSystem.h"
#include "TimerSystem.h"
#include "EntityFactory.h"
#include "EntityCreationPacket.h"

#include <DebugUtil.h>
#include <ToString.h>

TempModuleSpawner::TempModuleSpawner()
	: EntitySystem(SystemType::TempModuleSpawner)
{

}

TempModuleSpawner::~TempModuleSpawner()
{

}

void TempModuleSpawner::process()
{
	if (m_timerSystem->checkTimeInterval(TimerIntervals::EverySecond))
	{
		//DEBUGPRINT(("Request spawning a module at a random position.\n"));
		if (m_spawnPointSystem->isSpawnPointsReady())
		{
			AglMatrix pos = m_spawnPointSystem->getRandomFreeModuleSpawnPoint();

			while (! (pos == m_spawnPointSystem->invalidSpawnPoint()) )
			{
				EntityCreationPacket cp;

				int randModule = rand() % (EntityType::EndModule - EntityType::ShipModuleStart - 1);
				randModule += EntityType::ShipModuleStart + 1;

				cp.entityType = randModule;
				cp.scale = AglVector3(1.0f, 1.0f, 1.0f);
				m_factory->entityFromPacket(cp, &pos);
	
				AglVector3 posV = pos.GetTranslation();
				string posAsString = toString(posV.x) + " " + toString(posV.y) + " " + toString(posV.z) + "\n";
	
				DEBUGPRINT(((toString("Module spawned at position ") + posAsString).c_str()));

				pos = m_spawnPointSystem->getRandomFreeModuleSpawnPoint();
			}
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


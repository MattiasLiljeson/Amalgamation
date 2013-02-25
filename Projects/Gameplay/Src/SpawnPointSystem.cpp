#define FORCE_VS_DBG_OUTPUT

#include "SpawnPointSystem.h"
#include "SpawnPointSet.h"
#include "LevelPieceRoot.h"

SpawnPointSystem::SpawnPointSystem()
	: EntitySystem(SystemType::SpawnPointSystem, 1, ComponentType::SpawnPointSet)
{

}

SpawnPointSystem::~SpawnPointSystem()
{

}

void SpawnPointSystem::clearSpawnPoints()
{
	m_shipSpawnPoints.clear();
	m_moduleSpawnPoints.clear();
}

void SpawnPointSystem::inserted( Entity* p_entity )
{
	auto spawnPointSet = static_cast<SpawnPointSet*>(
		p_entity->getComponent(ComponentType::SpawnPointSet));
	
	vector<AglMatrix> newShipSpawnPoints;
	vector<AglMatrix> newModuleSpawnPoints;

	for (unsigned int i = 0; i < spawnPointSet->m_spawnPoints.size(); i++)
	{
		SpawnPoint spawnPoint = spawnPointSet->m_spawnPoints[i];

		if (spawnPoint.spAction == "ship")
		{
			newShipSpawnPoints.push_back(spawnPoint.spTransform);
		}
		else if (spawnPoint.spAction == "module")
		{
			newModuleSpawnPoints.push_back(spawnPoint.spTransform);
		}
	}

	// Group spawn points, even though one of the vectors is empty.
	if (newModuleSpawnPoints.size() + newShipSpawnPoints.size() > 0)
	{
		m_shipSpawnPoints.push_back(newShipSpawnPoints);
		m_moduleSpawnPoints.push_back(newModuleSpawnPoints);
	}
}

void SpawnPointSystem::processEntities( const vector<Entity*>& p_entities )
{
	
}

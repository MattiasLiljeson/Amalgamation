#define FORCE_VS_DBG_OUTPUT

#include "SpawnPointSystem.h"
#include "SpawnPointSet.h"
#include "EntityFactory.h"
#include "LevelHandlerSystem.h"
#include "LevelPieceRoot.h"

#include <DebugUtil.h>

SpawnPointSystem::SpawnPointSystem()
	: EntitySystem(SystemType::SpawnPointSystem, 2, ComponentType::SpawnPointSet, 
													ComponentType::LevelPieceRoot)
{

}

SpawnPointSystem::~SpawnPointSystem()
{

}

void SpawnPointSystem::initialize()
{
	m_entityFactory = static_cast<EntityFactory*>(
		m_world->getSystem(SystemType::EntitySystem));

	m_levelHandler = static_cast<LevelHandlerSystem*>(
		m_world->getSystem(SystemType::LevelHandlerSystem));


}

Entity* SpawnPointSystem::createModuleAtRandomSpawnPoint( EntityType::EntityEnums p_module )
{
	return NULL;
}

Entity* SpawnPointSystem::createShipAtRandomSpawnPoint()
{
	return NULL;
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
	
	auto pieceRoot = static_cast<LevelPieceRoot*>(
		p_entity->getComponent(ComponentType::LevelPieceRoot));

	int pieceId = pieceRoot->pieceId;
	if (pieceId >= 0)
	{
		vector<pair<AglMatrix, SpawnPointState>> newShipSpawnPoints;
		vector<pair<AglMatrix, SpawnPointState>> newModuleSpawnPoints;

		// Resize the spawnpoint vectors to make this chamber fit.
		if (pieceRoot->pieceId >= m_shipSpawnPoints.size())
		{
			m_shipSpawnPoints.resize(pieceId + 1);
			m_moduleSpawnPoints.resize(pieceId + 1);
		}

		for (unsigned int i = 0; i < spawnPointSet->m_spawnPoints.size(); i++)
		{
			SpawnPoint spawnPoint = spawnPointSet->m_spawnPoints[i];

			if (spawnPoint.spAction == "ship")
			{
				newShipSpawnPoints.push_back(make_pair(spawnPoint.spTransform, SPAWNPOINTSTATE_FREE));
			}
			else if (spawnPoint.spAction == "module")
			{
				newModuleSpawnPoints.push_back(make_pair(spawnPoint.spTransform, SPAWNPOINTSTATE_FREE));
			}
		}
		m_shipSpawnPoints[pieceId]		= newShipSpawnPoints;
		m_moduleSpawnPoints[pieceId]	= newModuleSpawnPoints;
		
		// Remove the spawn point set, so that it cannot be used by other systems
		p_entity->removeComponent(ComponentType::SpawnPointSet);
		p_entity->applyComponentChanges();
	}
	else
	{
		DEBUGPRINT(("Warning: SpawnPointSystem received an entity that contains an invalid levelId.\n"));
	}
}

void SpawnPointSystem::processEntities( const vector<Entity*>& p_entities )
{
	
}

const AglMatrix& SpawnPointSystem::getRandomFreeShipSpawnPoint()
{
	auto selectId = getRandomFreeSpawnPoint(m_shipSpawnPoints);
	setSpawnPointState(m_shipSpawnPoints, selectId.first, selectId.second, SPAWNPOINTSTATE_OCCUPIED);
	
	if (selectId.first >= 0 && selectId.second >= 0)
		return m_shipSpawnPoints[selectId.first][selectId.second].first;
	else
	{
		DEBUGPRINT(("Warning: Found no available ship spawnpoint. Identity matrix is returned."));
		return AglMatrix::identityMatrix();
	}
}

const AglMatrix& SpawnPointSystem::getRandomFreeModuleSpawnPoint()
{
	auto selectId = getRandomFreeSpawnPoint(m_moduleSpawnPoints);
	setSpawnPointState(m_moduleSpawnPoints, selectId.first, selectId.second, SPAWNPOINTSTATE_OCCUPIED);
	
	if (selectId.first >= 0 && selectId.second >= 0)
		return m_moduleSpawnPoints[selectId.first][selectId.second].first;
	else
	{
		DEBUGPRINT(("Warning: Found no available module spawnpoint. Identity matrix is returned."));
		return AglMatrix::identityMatrix();
	}
}

pair<int, int> SpawnPointSystem::getRandomFreeSpawnPoint( 
	const vector<vector<pair<AglMatrix, SpawnPointState>>>& p_fromSpawnPoints ) const
{
	vector<pair<int, int>> selection;

	for (int chamber = 0; chamber < p_fromSpawnPoints.size(); chamber++)
	{
		for (int point = 0; point < p_fromSpawnPoints[chamber].size(); point++)
		{
			if (p_fromSpawnPoints[chamber][point].second == SPAWNPOINTSTATE_FREE)
			{
				selection.push_back(make_pair(chamber, point));
			}
		}
	}

	if (selection.size() > 0)
	{
		int randIndex = rand() % selection.size();
		return make_pair(selection[randIndex].first, selection[randIndex].second);
	}
	else
		return make_pair(-1, -1);
}

void SpawnPointSystem::setSpawnPointState( 
	vector<vector<pair<AglMatrix, SpawnPointState>>>& p_inSpawnPoints, 
	int p_inChamber, int p_inPoint, SpawnPointState p_newState )
{
	p_inSpawnPoints[p_inChamber][p_inPoint].second = p_newState;
}






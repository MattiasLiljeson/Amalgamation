#define FORCE_VS_DBG_OUTPUT

#include "SpawnPointSystem.h"
#include "SpawnPointSet.h"
#include "EntityFactory.h"
#include "LevelHandlerSystem.h"
#include "LevelPieceRoot.h"
#include "Transform.h"

#include <ToString.h>
#include <DebugUtil.h>

SpawnPointSystem::SpawnPointSystem()
	: EntitySystem(SystemType::SpawnPointSystem, 3, ComponentType::SpawnPointSet, 
													ComponentType::LevelPieceRoot,
													ComponentType::Transform)
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
		vector<ShipSpawnPoint> newShipSpawnPoints;
		vector<ModuleSpawnPoint> newModuleSpawnPoints;

		DEBUGPRINT(( (toString("Starting to read spawnpoint of chamber ") + toString(pieceId) + " " + p_entity->getName() + "\n").c_str()) );
		auto rootTransform = static_cast<Transform*>(
			p_entity->getComponent(ComponentType::Transform));

		// Resize the spawnpoint vectors to make this chamber fit.
		if (pieceRoot->pieceId >= m_shipSpawnPoints.size())
		{
			m_shipSpawnPoints.resize(pieceId + 1);
			m_moduleSpawnPoints.resize(pieceId + 1);
		}

		for (unsigned int i = 0; i < spawnPointSet->m_spawnPoints.size(); i++)
		{
			SpawnPoint spawnPoint = spawnPointSet->m_spawnPoints[i];
			// Transform spawnPoint to world space.
			spawnPoint.spTransform *= rootTransform->getMatrix();

			if (spawnPoint.spAction == "ship")
			{
				AglVector3 posV = spawnPoint.spTransform.GetTranslation();
				string posAsString = toString(posV.x) + " " + toString(posV.y) + " " + toString(posV.z) + "\n";

				DEBUGPRINT(( (toString("Ship spawnpoint found at position ") + posAsString).c_str()) );
				newShipSpawnPoints.push_back(ShipSpawnPoint(spawnPoint.spTransform));
			}
			else if (spawnPoint.spAction == "module")
			{
				AglVector3 posV = spawnPoint.spTransform.GetTranslation();
				string posAsString = toString(posV.x) + " " + toString(posV.y) + " " + toString(posV.z) + "\n";

				DEBUGPRINT(( (toString("Module spawnpoint found at position ") + posAsString).c_str()) );

				newModuleSpawnPoints.push_back(ModuleSpawnPoint(spawnPoint.spTransform));
			}
		}
		m_shipSpawnPoints[pieceId]		= newShipSpawnPoints;
		m_moduleSpawnPoints[pieceId]	= newModuleSpawnPoints;
		
		// Remove the spawn point set, so that it cannot be used by other systems
		p_entity->removeComponent(ComponentType::SpawnPointSet);
		p_entity->applyComponentChanges();

		DEBUGPRINT(("Finished reading spawnpoints from chamber.\n"));
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

	if (selectId.first >= 0 && selectId.second >= 0)
	{
		setSpawnPointState(m_shipSpawnPoints, selectId.first, selectId.second, SPAWNPOINTSTATE_OCCUPIED);
		return m_shipSpawnPoints[selectId.first][selectId.second].transform;
	}
	else
	{
		//DEBUGPRINT(("Warning: Found no available ship spawnpoint. An invalidSpawnPoint is returned.\n"));
		return invalidSpawnPoint();
	}
}

const AglMatrix& SpawnPointSystem::getRandomFreeModuleSpawnPoint()
{
	auto selectId = getRandomFreeSpawnPoint(m_moduleSpawnPoints);

	if (selectId.first >= 0 && selectId.second >= 0)
	{
		setSpawnPointState(m_moduleSpawnPoints, selectId.first, selectId.second, SPAWNPOINTSTATE_OCCUPIED);
		return m_moduleSpawnPoints[selectId.first][selectId.second].transform;
	}
	else
	{
		//DEBUGPRINT(("Warning: Found no available module spawnpoint. An invalidSpawnPoint is returned.\n"));
		return invalidSpawnPoint();
	}
}

pair<int, int> SpawnPointSystem::getRandomFreeSpawnPoint( 
	const vector<vector<ModuleOrShipSpawnPoint>>& p_fromSpawnPoints ) const
{
	vector<pair<int, int>> selection;

	for (int chamber = 0; chamber < p_fromSpawnPoints.size(); chamber++)
	{
		for (int point = 0; point < p_fromSpawnPoints[chamber].size(); point++)
		{
			if (p_fromSpawnPoints[chamber][point].state == SPAWNPOINTSTATE_FREE)
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
	vector<vector<ModuleOrShipSpawnPoint>>& p_inSpawnPoints, 
	int p_inChamber, int p_inPoint, SpawnPointState p_newState )
{
	p_inSpawnPoints[p_inChamber][p_inPoint].state = p_newState;
}

bool SpawnPointSystem::isSpawnPointsReady() const
{
	return m_levelHandler->hasLevel();
}

AglMatrix SpawnPointSystem::invalidSpawnPoint() const
{
	return AglMatrix(AglVector3::one(), AglQuaternion::identity(), 
		AglVector3(FLT_MAX, FLT_MAX, FLT_MAX));
}








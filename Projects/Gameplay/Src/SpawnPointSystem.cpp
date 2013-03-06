#define FORCE_VS_DBG_OUTPUT

#include "SpawnPointSystem.h"
#include "SpawnPointSet.h"
#include "EntityFactory.h"
#include "LevelHandlerSystem.h"
#include "LevelPieceRoot.h"
#include "Transform.h"
#include "TimerSystem.h"
#include <OutputLogger.h>

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
	clearSpawnPoints();
}

void SpawnPointSystem::initialize()
{
	m_levelHandler = static_cast<LevelHandlerSystem*>(
		m_world->getSystem(SystemType::LevelHandlerSystem));

	m_timerSystem = static_cast<TimerSystem*>(
		m_world->getSystem(SystemType::TimerSystem));
}

void SpawnPointSystem::clearSpawnPoints()
{
	for (unsigned int chamberId = 0; chamberId < m_shipSpawnPoints.size(); chamberId++)
	{
		for (unsigned int pointId = 0; pointId < m_shipSpawnPoints[chamberId].size(); pointId++)
		{
			delete m_shipSpawnPoints[chamberId][pointId];
		}
	}
	for (unsigned int chamberId = 0; chamberId < m_moduleSpawnPoints.size(); chamberId++)
	{
		for (unsigned int pointId = 0; pointId < m_moduleSpawnPoints[chamberId].size(); pointId++)
		{
			delete m_moduleSpawnPoints[chamberId][pointId];
		}
	}

	m_refreshingSpawnPoints.clear();
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
		vector<ShipSpawnPointData*> newShipSpawnPoints;
		vector<ModuleSpawnPointData*> newModuleSpawnPoints;

		m_world->getOutputLogger()
			->write((toString("Starting to read spawnpoint of chamber ") + toString(pieceId) + " " + p_entity->getName() + "\n").c_str());
		
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

				//DEBUGPRINT(( (toString("Ship spawnpoint found at position ") + posAsString).c_str()) );
				newShipSpawnPoints.push_back(new ShipSpawnPointData(spawnPoint.spTransform, pieceRoot->pieceId, newShipSpawnPoints.size()));
			}
			else if (spawnPoint.spAction == "module")
			{
				AglVector3 posV = spawnPoint.spTransform.GetTranslation();
				string posAsString = toString(posV.x) + " " + toString(posV.y) + " " + toString(posV.z) + "\n";

				//DEBUGPRINT(( (toString("Module spawnpoint found at position ") + posAsString).c_str()) );

				newModuleSpawnPoints.push_back(new ModuleSpawnPointData(spawnPoint.spTransform, pieceRoot->pieceId, newModuleSpawnPoints.size()));
			}
		}
		m_shipSpawnPoints[pieceId]		= newShipSpawnPoints;
		m_moduleSpawnPoints[pieceId]	= newModuleSpawnPoints;
		
		// Remove the spawn point set, so that it cannot be used by other systems
		p_entity->removeComponent(ComponentType::SpawnPointSet);
		p_entity->applyComponentChanges();

		string debugPrintStr = "Found ";
		debugPrintStr += toString(newShipSpawnPoints.size());
		debugPrintStr += " ship spawnpoints.\nFound ";
		debugPrintStr += toString(newModuleSpawnPoints.size());
		debugPrintStr += " module spawnpoints.\n";

		m_world->getOutputLogger()
			->write(debugPrintStr.c_str());
	}
	else
	{
		m_world->getOutputLogger()
			->write("SpawnPointSystem received an entity that contains an invalid levelId.\n", WRITETYPE_WARNING);
	}
}

void SpawnPointSystem::processEntities( const vector<Entity*>& p_entities )
{
	if (m_timerSystem->checkTimeInterval(TimerIntervals::EverySecond))
	{
		int spawnPointId = 0;
		while (spawnPointId < m_refreshingSpawnPoints.size())
		{
			m_refreshingSpawnPoints[spawnPointId]->currentCooldown -= 1.0f;
			if (m_refreshingSpawnPoints[spawnPointId]->currentCooldown <= 0.0f)
			{
				string debugPrintStr = "Spawnpoint cooldown is done.\n\t@chamber: ";
				debugPrintStr += toString(m_refreshingSpawnPoints[spawnPointId]->inChamber);
				debugPrintStr += "\n\t@spawnpoint: ";
				debugPrintStr += toString(m_refreshingSpawnPoints[spawnPointId]->atSpawnPoint);
				debugPrintStr += "\n";
				m_world->getOutputLogger()
					->write(debugPrintStr.c_str());

				m_refreshingSpawnPoints[spawnPointId]->state = SPAWNPOINTSTATE_FREE;
				m_refreshingSpawnPoints[spawnPointId] = m_refreshingSpawnPoints.back();
				m_refreshingSpawnPoints.pop_back();
			}
			else
				spawnPointId++;
		}
	}
}

const AglMatrix& SpawnPointSystem::getRandomFreeShipSpawnPoint()
{
	auto selectId = getRandomFreeSpawnPoint(m_shipSpawnPoints);

	if (selectId.first >= 0 && selectId.second >= 0)
	{
		setSpawnPointState(m_shipSpawnPoints, selectId.first, selectId.second, SPAWNPOINTSTATE_OCCUPIED);
		return m_shipSpawnPoints[selectId.first][selectId.second]->transform;
	}
	else
	{
		//DEBUGPRINT(("Warning: Found no available ship spawnpoint. An invalidSpawnPoint is returned.\n"));
		return invalidSpawnPoint();
	}
}

const AglMatrix& SpawnPointSystem::getRandomFreeModuleSpawnPoint()
{
	return getRandomFreeModuleSpawnPointData()->transform;
}

const ModuleSpawnPointData* SpawnPointSystem::getRandomFreeModuleSpawnPointData()
{
	auto selectId = getRandomFreeSpawnPoint(m_moduleSpawnPoints);

	if (selectId.first >= 0 && selectId.second >= 0)
	{
		setSpawnPointState(m_moduleSpawnPoints, selectId.first, selectId.second, SPAWNPOINTSTATE_OCCUPIED);
		return m_moduleSpawnPoints[selectId.first][selectId.second];
	}
	else
	{
		//DEBUGPRINT(("Warning: Found no available module spawnpoint data. An invalidSpawnPointData is returned.\n"));
		return NULL;
	}
}

pair<int, int> SpawnPointSystem::getRandomFreeSpawnPoint( 
	const vector<vector<ModuleOrShipSpawnPointData*>>& p_fromSpawnPoints ) const
{
	vector<pair<int, int>> selection;

	for (int chamber = 0; chamber < p_fromSpawnPoints.size(); chamber++)
	{
		for (int point = 0; point < p_fromSpawnPoints[chamber].size(); point++)
		{
			if (p_fromSpawnPoints[chamber][point]->state == SPAWNPOINTSTATE_FREE)
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
	vector<vector<ModuleOrShipSpawnPointData*>>& p_inSpawnPoints, 
	int p_inChamber, int p_inPoint, SpawnPointState p_newState )
{
	p_inSpawnPoints[p_inChamber][p_inPoint]->state = p_newState;
}

bool SpawnPointSystem::isSpawnPointsReady() const
{
	return m_levelHandler->hasLevel();
}

const AglMatrix& SpawnPointSystem::invalidSpawnPoint()
{
	return s_invalidSpawnPoint;
}

void SpawnPointSystem::applyResetCooldown( int p_atChamber, int p_atSpawnPoint )
{
	ModuleSpawnPointData* spawnPoint = m_moduleSpawnPoints[p_atChamber][p_atSpawnPoint];
	spawnPoint->currentCooldown = 10.0f;
	m_refreshingSpawnPoints.push_back(spawnPoint);
}

const AglMatrix SpawnPointSystem::s_invalidSpawnPoint(AglVector3::one(), 
													AglQuaternion::identity(), 
													AglVector3(FLT_MAX, FLT_MAX, FLT_MAX));

const ModuleOrShipSpawnPointData SpawnPointSystem::s_invalidSpawnPointData(s_invalidSpawnPoint, -1, -1);







#pragma once

#include <EntitySystem.h>
#include <AglMatrix.h>
#include <vector>
#include "EntityType.h"
using namespace std;

class EntityFactory;
class LevelHandlerSystem;

enum SpawnPointState
{
	SPAWNPOINTSTATE_FREE,
	SPAWNPOINTSTATE_OCCUPIED,
};

// =======================================================================================
//                                      SpawnPointSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	This system processes spawn point set components that define ship and module 
/// spawn points.
///        
/// # SpawnPointSystem
/// The system stores the spawn points in a vector within a vector. This is to give the
/// functionality to be able to spawn things depending on which chamber it is in.
/// Created on: 25-2-2013 
///---------------------------------------------------------------------------------------

class SpawnPointSystem : public EntitySystem
{
public:
	SpawnPointSystem();
	virtual ~SpawnPointSystem();

	void initialize();
	void clearSpawnPoints();

	/// Creates the desired module type at a random module spawnpoint location.
	Entity* createModuleAtRandomSpawnPoint(EntityType::EntityEnums p_module);
	Entity* createShipAtRandomSpawnPoint();

	const AglMatrix& getRandomFreeShipSpawnPoint();
	const AglMatrix& getRandomFreeModuleSpawnPoint();

	virtual void inserted( Entity* p_entity );

	virtual void processEntities( const vector<Entity*>& p_entities );

	
private:
	pair<int, int> getRandomFreeSpawnPoint(
		const vector<vector<pair<AglMatrix, SpawnPointState>>>& p_fromSpawnPoints) const;
	void setSpawnPointState(
		vector<vector<pair<AglMatrix, SpawnPointState>>>& p_inSpawnPoints,
		int p_inChamber, int p_inPoint, SpawnPointState p_newState);

	// Stores ship spawn points and module spawn points. The bool defines whether or not
	// the spawn point is free (free = true, occupied = false)
	vector<vector<pair<AglMatrix, SpawnPointState>>> m_shipSpawnPoints;
	vector<vector<pair<AglMatrix, SpawnPointState>>> m_moduleSpawnPoints;

	vector<int> freeSlots;

	EntityFactory*		m_entityFactory;
	LevelHandlerSystem* m_levelHandler;
};
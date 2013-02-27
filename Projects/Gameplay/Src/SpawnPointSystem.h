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

struct ModuleOrShipSpawnPoint
{
	ModuleOrShipSpawnPoint(const AglMatrix& p_transform)
	{
		transform	= p_transform;
		state		= SPAWNPOINTSTATE_FREE;
	}

	AglMatrix		transform;
	SpawnPointState state;
};

typedef ModuleOrShipSpawnPoint ShipSpawnPoint;
typedef ModuleOrShipSpawnPoint ModuleSpawnPoint;

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

	const AglMatrix& getRandomFreeShipSpawnPoint();
	const AglMatrix& getRandomFreeModuleSpawnPoint();

	virtual void inserted( Entity* p_entity );

	virtual void processEntities( const vector<Entity*>& p_entities );

	bool isSpawnPointsReady() const;
	
	static const AglMatrix& invalidSpawnPoint();
private:
	pair<int, int> getRandomFreeSpawnPoint(
		const vector<vector<ModuleOrShipSpawnPoint>>& p_fromSpawnPoints) const;
	void setSpawnPointState(
		vector<vector<ModuleOrShipSpawnPoint>>& p_inSpawnPoints,
		int p_inChamber, int p_inPoint, SpawnPointState p_newState);
private:

	// Stores ship spawn points and module spawn points. The bool defines whether or not
	// the spawn point is free (free = true, occupied = false)
	vector<vector<ShipSpawnPoint>> m_shipSpawnPoints;
	vector<vector<ModuleSpawnPoint>> m_moduleSpawnPoints;

	vector<int> freeSlots;

	LevelHandlerSystem* m_levelHandler;
	static const AglMatrix s_invalidSpawnPoint;
};
#pragma once

#include <EntitySystem.h>
#include <AglMatrix.h>
#include <vector>
#include "EntityType.h"
using namespace std;

class EntityFactory;
class LevelHandlerSystem;
class TimerSystem;
class LevelGenSystem;

enum SpawnPointState
{
	SPAWNPOINTSTATE_FREE,
	SPAWNPOINTSTATE_OCCUPIED,
};

struct ModuleOrShipSpawnPointData
{
	ModuleOrShipSpawnPointData(const AglMatrix& p_transform, int p_inChamber, int p_atSpawnPoint)
	{
		transform		= p_transform;
		state			= SPAWNPOINTSTATE_FREE;
		currentCooldown	= 0.0f;
		inChamber		= p_inChamber;
		atSpawnPoint	= p_atSpawnPoint;
	}

	AglMatrix		transform;
	SpawnPointState state;
	float			currentCooldown;
	int				inChamber;
	int				atSpawnPoint;

};

typedef ModuleOrShipSpawnPointData ShipSpawnPointData;
typedef ModuleOrShipSpawnPointData ModuleSpawnPointData;

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

	const ModuleSpawnPointData* getRandomFreeModuleSpawnPointData();

	virtual void inserted( Entity* p_entity );

	virtual void processEntities( const vector<Entity*>& p_entities );

	bool isSpawnPointsReady() const;

	void applyResetCooldown(int p_atChamber, int p_atSpawnPoint);

	static const AglMatrix& invalidSpawnPoint();
private:
	pair<int, int> getRandomFreeSpawnPoint(
		const vector<vector<ModuleOrShipSpawnPointData*>>& p_fromSpawnPoints) const;
	void setSpawnPointState(
		vector<vector<ModuleOrShipSpawnPointData*>>& p_inSpawnPoints,
		int p_inChamber, int p_inPoint, SpawnPointState p_newState);
private:

	// Stores ship spawn points and module spawn points. The bool defines whether or not
	// the spawn point is free (free = true, occupied = false)
	vector<vector<ShipSpawnPointData*>> m_shipSpawnPoints;
	vector<vector<ModuleSpawnPointData*>> m_moduleSpawnPoints;

	vector<ModuleSpawnPointData*> m_refreshingSpawnPoints;

	vector<int> freeSlots;

	LevelHandlerSystem* m_levelHandler;
	LevelGenSystem*		m_levelGen;
	TimerSystem*		m_timerSystem;

	static const AglMatrix s_invalidSpawnPoint;
	static const ModuleOrShipSpawnPointData s_invalidSpawnPointData;
};
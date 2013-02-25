#pragma once

#include <EntitySystem.h>
#include <AglMatrix.h>
#include <vector>
using namespace std;

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

	void clearSpawnPoints();

	virtual void inserted( Entity* p_entity );

	virtual void processEntities( const vector<Entity*>& p_entities );

	
private:
	vector<vector<AglMatrix>> m_shipSpawnPoints;
	vector<vector<AglMatrix>> m_moduleSpawnPoints;
};
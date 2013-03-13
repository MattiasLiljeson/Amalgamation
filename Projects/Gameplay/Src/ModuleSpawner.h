#pragma once

#include <EntitySystem.h>

class SpawnPointSystem;
class TimerSystem;
class EntityFactory;
class TcpServer;
// =======================================================================================
//                                      ModuleSpawner
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	This system allows the developer to spawn modules at random unoccupied spawn points.
/// This is to ensure that the functionality works.
///        
/// # ManualModuleSpawner
/// Detailed description.....
/// Created on: 27-2-2013 
///---------------------------------------------------------------------------------------

class ModuleSpawner : public EntitySystem
{
public:
	ModuleSpawner(TcpServer* p_server);
	virtual ~ModuleSpawner();

	virtual void process();

	virtual void initialize();

	virtual void processEntities( const vector<Entity*>& p_entities );

	virtual void inserted( Entity* p_entity );

	virtual void removed( Entity* p_entity );



protected:
private:
	TcpServer*			m_server;
	SpawnPointSystem*	m_spawnPointSystem;
	TimerSystem*		m_timerSystem;
	EntityFactory*		m_factory;

	int m_spawnedModulesCount;
	int m_spawnedModulesMax;
};
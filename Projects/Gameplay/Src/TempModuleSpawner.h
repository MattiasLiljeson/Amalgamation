#pragma once

#include <EntitySystem.h>

class SpawnPointSystem;
class TimerSystem;
class EntityFactory;
class TcpServer;
// =======================================================================================
//                                      TempModuleSpawner
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	This system allows the developer to spawn modules at random unoccupied spawn points.
/// This is to ensure that the functionality works.
///        
/// # ManualModuleSpawner
/// Detailed description.....
/// Created on: 27-2-2013 
///---------------------------------------------------------------------------------------

class TempModuleSpawner : public EntitySystem
{
public:
	TempModuleSpawner(TcpServer* p_server);
	virtual ~TempModuleSpawner();

	virtual void process();

	virtual void initialize();
protected:
private:
	TcpServer*			m_server;
	SpawnPointSystem*	m_spawnPointSystem;
	TimerSystem*		m_timerSystem;
	EntityFactory*		m_factory;

};
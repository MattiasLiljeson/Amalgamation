#include "SystemManager.h"

SystemManager::SystemManager( EntityWorld* p_world )
{
	m_world = p_world;
}

SystemManager::~SystemManager()
{
}

EntitySystem* SystemManager::getSystem( SystemType::SystemTypeIdx p_systemIndex )
{
	return m_systems[p_systemIndex];
}

void SystemManager::setSystem( EntitySystem* p_system, SystemType::SystemTypeIdx p_index )
{
	m_systems[p_index] = p_system;
}

void SystemManager::initializeAll()
{
	map<SystemType::SystemTypeIdx, EntitySystem*>::iterator it;
	for( it=m_systems.begin(); it != m_systems.end(); it++ )
		it->second->initialize();
}

void SystemManager::updateSynchronous()
{
	map<SystemType::SystemTypeIdx, EntitySystem*>::iterator it;
	for( it=m_systems.begin(); it != m_systems.end(); it++ )
		it->second->process();
}

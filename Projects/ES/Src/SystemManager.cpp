#include "SystemManager.h"

SystemManager::SystemManager( EntityWorld* p_world )
{
	m_world = p_world;
}

SystemManager::~SystemManager()
{
}

EntitySystem* SystemManager::getSystem( type_info p_system )
{
	return m_systems[p_system];
}

void SystemManager::setSystem( EntitySystem* p_system )
{
	m_systems[typeid(p_system)] = p_system;
}

void SystemManager::initializeAll()
{
	map<type_info, EntitySystem*>::iterator it;
	for( it=m_systems.begin(); it != m_systems.end(); it++ )
		it->second->initialize();
}

void SystemManager::updateSynchronous()
{
	map<type_info, EntitySystem*>::iterator it;
	for( it=m_systems.begin(); it != m_systems.end(); it++ )
		it->second->process();
}

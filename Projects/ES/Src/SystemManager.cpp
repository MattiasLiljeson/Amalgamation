#include "SystemManager.h"

SystemManager::SystemManager( EntityWorld* p_world )
{
	m_world = p_world;
}

SystemManager::~SystemManager()
{
	map<SystemType::SystemTypeIdx, EntitySystem*>::iterator it;
	for( it=m_systems.begin(); it != m_systems.end(); it++ )
	{
		delete it->second;
		it->second = NULL;
	}
}

EntitySystem* SystemManager::getSystem( SystemType::SystemTypeIdx p_systemIndex )
{
	return m_systems[p_systemIndex];
}

EntitySystem* SystemManager::setSystem( SystemType p_type, EntitySystem* p_system,
									   bool p_enabled )
{
	p_system->setEnabled( p_enabled );
	m_systems[(SystemType::SystemTypeIdx)p_type.getIndex()] = p_system;
	return p_system;
}

EntitySystem* SystemManager::setSystem( SystemType::SystemTypeIdx p_typeIdx,
									   EntitySystem* p_system, bool p_enabled )
{
	p_system->setEnabled( p_enabled );
	m_systems[p_typeIdx] = p_system;
	return p_system;
}

void SystemManager::deleteSystem( SystemType p_type )
{
	deleteSystem( (SystemType::SystemTypeIdx)p_type.getIndex() );
}

void SystemManager::deleteSystem( SystemType::SystemTypeIdx p_typeIdx )
{
	delete m_systems[p_typeIdx];
	m_systems[p_typeIdx] = NULL;
}

void SystemManager::deleteSystem( EntitySystem* p_system)
{
	//HACK: break in for-loop 
	map<SystemType::SystemTypeIdx, EntitySystem*>::iterator it;
	for( it=m_systems.begin(); it != m_systems.end(); it++ )
	{
		if( it->second == p_system)
		{
			delete p_system;
			it->second = NULL;
			break;
		}
	}
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

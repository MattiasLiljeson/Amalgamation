#include "EntityWorld.h"


EntityWorld::EntityWorld()
{
	m_componentManager = new ComponentManager();
	setManager( Manager::ComponentManager, static_cast<Manager*>(m_componentManager) );
		
	m_entityManager = new EntityManager();
	setManager(  Manager::EntityManager, static_cast<Manager*>(m_entityManager) );
}


EntityWorld::~EntityWorld()
{
}

void EntityWorld::initialize()
{
	for (int i = 0; i < m_managersBag.size(); i++)
			m_managersBag[i]->initialize();
		
	for (int i = 0; i < m_systemsBag.size(); i++)
	{
		// Need this?
		//ComponentMapperInitHelper.config(systemsBag.get(i), this);
		m_systemsBag[i]->initialize();
	}
}

EntityManager* EntityWorld::getEntityManager()
{
	return m_entityManager;
}

ComponentManager* EntityWorld::getComponentManager()
{
	return m_componentManager;
}

Manager* EntityWorld::setManager( Manager::ManagerType p_managerType, Manager* p_manager )
{
	m_managers.reserve(p_managerType+1); // index+1 = required size 
	m_managers[p_managerType] = p_manager;
	m_managersBag.push_back(p_manager);

}

Manager* EntityWorld::getManager( Manager::ManagerType p_managerType )
{
	return m_managers[p_managerType];
}

void EntityWorld::deleteManager( Manager* p_manager )
{
	// Find the correct manager-object in the vector and delete it from both the vector
	// and the bag by using the other overloaded variant of this function;
	// HACK: break in for-loop below
	for( int i=0; i<m_managers.size(); i++)
	{
		if(m_managers[i] == p_manager)
		{
			deleteManager((Manager::ManagerType)i);
			break;
		}
	}
}

void EntityWorld::deleteManager( Manager::ManagerType p_managerType )
{
	// Find the correct manager-object in the bag and delete it;
	// HACK: break in for-loop below
	for( int i=0; i<m_managersBag.size(); i++)
	{
		if( m_managersBag[i] == m_managers[p_managerType])
		{
			delete m_managersBag[i];
			m_managersBag[i] = NULL;
			m_managers[p_managerType] = NULL;
			break;
		}
	}
}

float EntityWorld::getDelta()
{
	return m_delta;
}

void EntityWorld::setDelta( float p_dt )
{
	m_delta = p_dt;
}

void EntityWorld::addEntity( Entity* p_entity )
{
	m_added.push_back(p_entity);
}

void EntityWorld::changedEntity( Entity* p_entity )
{
	m_changed.push_back(p_entity);
}

void EntityWorld::deleteEntity( Entity* p_entity )
{
	// Add only to vector if not already in the m_deleted vector
	// HACK: Early return in for-loop below!
	for( int i=0; i<m_deleted.size(); i++ )
		if( m_deleted[i] == p_entity )
			return;

	m_deleted.push_back(p_entity);
}

void EntityWorld::enable( Entity* p_entity )
{
	m_enable.push_back(p_entity);
}

void EntityWorld::disable( Entity* p_entity )
{
	m_disable.push_back(p_entity);
}

Entity* EntityWorld::createEntity()
{
	return m_entityManager->createEntityInstance();
}

Entity* EntityWorld::getEntity( int p_entityId )
{
	return m_entityManager->getEntity( p_entityId );
}

//Systems

void EntityWorld::notifyManagers( IPerformer* p_performer, Entity* p_entity )
{
	for(int i=0; i<m_managersBag.size(); i++)
		p_performer->perform(m_managersBag[i], p_entity);
}

EntitySystem* EntityWorld::getSystem( SystemType p_type )
{
	return getSystem( (SystemType::SystemTypeIdx)p_type.getIndex() );
}

EntitySystem* EntityWorld::getSystem( SystemType::SystemTypeIdx p_typeIdx )
{
	return m_systemManager->getSystem( p_typeIdx );
}

void EntityWorld::check( vector<Entity*> p_entities, IPerformer* p_performer )
{
	if(!p_entities.empty())
	{
		for (int i=0; i<p_entities.size(); i++)
		{
			Entity* entity = p_entities[i];
			notifyManagers(p_performer, entity);
			notifySystems(p_performer, entity);
		}
		p_entities.clear();
	}

	// HACK: Performance killer. Result of direct port of Java impl.
	delete p_performer;
}

void EntityWorld::process()
{
	check( m_added,   new AddedPerformer );
	check( m_changed, new ChangedPerformer );
	check( m_disable, new DisabledPerformer );
	check( m_enable,  new EnabledPerformer );
	check( m_deleted, new DeletedPerformer );

	m_componentManager->clean();

	for(int i = 0; i<m_systemsBag.size(); i++) 
	{
		EntitySystem* system = m_systemsBag[i];
		if(system->getEnabled())
			system->process();
	}
}
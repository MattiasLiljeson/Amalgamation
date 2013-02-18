#include "EntityWorld.h"


EntityWorld::EntityWorld()
{
	m_totalGameTime = 0;
	m_delta = 0.01f;
	m_shutdown = false;
	m_aspectRatio = 800.0f/600.0f;

	m_componentManager = new ComponentManager();
	setManager( Manager::ComponentManager, m_componentManager );
		
	m_entityManager = new EntityManager();
	setManager(  Manager::EntityManager, m_entityManager );

	m_systemManager = new SystemManager( this );
	setManager(  Manager::SystemManager, m_systemManager );
}


EntityWorld::~EntityWorld()
{
	delete m_componentManager;
	m_componentManager = NULL;
	delete m_entityManager;
	m_entityManager = NULL;
	delete m_systemManager;
	m_systemManager = NULL;

	//m_systemsBag.clear();
}

void EntityWorld::initialize()
{
	for ( unsigned int i = 0; i < m_managersBag.size(); i++ )
			m_managersBag[i]->initialize();
}

EntityManager* EntityWorld::getEntityManager()
{
	return m_entityManager;
}

ComponentManager* EntityWorld::getComponentManager()
{
	return m_componentManager;
}

Manager* EntityWorld::setManager( Manager::ManagerTypeIdx p_managerType, Manager* p_manager )
{
	int reqSize = p_managerType+1;  // index+1 = required size 
	if(m_managers.size() < (unsigned int)reqSize)
		m_managers.resize(p_managerType+1);
	
	p_manager->setWorld(this);
	m_managers[p_managerType] = p_manager;
	m_managersBag.push_back(p_manager);
	return p_manager;
}

Manager* EntityWorld::getManager( Manager::ManagerTypeIdx p_managerType )
{
	return m_managers[p_managerType];
}

void EntityWorld::deleteManager( Manager* p_manager )
{
	// Find the correct manager-object in the vector and delete it from both the vector
	// and the bag by using the other overloaded variant of this function;
	//NOTE: break in for-loop 
	for( unsigned int i=0; i<m_managers.size(); i++ )
	{
		if(m_managers[i] == p_manager)
		{
			deleteManager((Manager::ManagerTypeIdx)i);
			break;
		}
	}
}

void EntityWorld::deleteManager( Manager::ManagerTypeIdx p_managerType )
{
	// Find the correct manager-object in the bag and delete it;
	//NOTE: break in for-loop 
	for( unsigned int i=0; i<m_managersBag.size(); i++ )
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

float EntityWorld::getElapsedTime()
{
	return m_totalGameTime;
}

float EntityWorld::getDelta()
{
	return m_delta;
}

void EntityWorld::setDelta( float p_dt )
{
	m_delta = p_dt;
}

float EntityWorld::getAspectRatio()
{
	return m_aspectRatio;
}

void EntityWorld::setAspectRatio(float p_aspectRatio)
{
	m_aspectRatio=p_aspectRatio;
}

void EntityWorld::addEntity( Entity* p_entity )
{
	if( p_entity != NULL )
	{
		// Add only to vector if not already in the m_added vector
		//NOTE: early return in for-loop 
		for( unsigned int i=0; i<m_added.size(); i++ )
			if( m_added[i] == p_entity )
				return;

		m_added.push_back(p_entity);
	}
}

void EntityWorld::changedEntity( Entity* p_entity )
{
	if( p_entity != NULL )
	{
		// Add only to vector if not already in the m_changed vector
		//NOTE: early return in for-loop 
		for( unsigned int i=0; i<m_changed.size(); i++ )
			if( m_changed[i] == p_entity )
				return;

		m_changed.push_back(p_entity);
	}
}

void EntityWorld::deleteEntity( Entity* p_entity )
{
	if( p_entity != NULL )
	{
		// Add only to vector if not already in the m_deleted vector
		//NOTE: early return in for-loop 
		for( unsigned int i=0; i<m_deleted.size(); i++ )
			if( m_deleted[i] == p_entity )
				return;

		m_deleted.push_back(p_entity);
	}
}

void EntityWorld::enable( Entity* p_entity )
{
	if( p_entity != NULL )
	{
		// Add only to vector if not already in the m_enable vector
		//NOTE: early return in for-loop 
		for( unsigned int i=0; i<m_enable.size(); i++ )
			if( m_enable[i] == p_entity )
				return;

		m_enable.push_back(p_entity);
	}
}

void EntityWorld::disable( Entity* p_entity )
{
	if( p_entity != NULL )
	{
		// Add only to vector if not already in the m_disable vector
		//NOTE: early return in for-loop 
		for( unsigned int i=0; i<m_deleted.size(); i++ )
			if( m_disable[i] == p_entity )
				return;

		m_disable.push_back(p_entity);
	}
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
SystemManager* EntityWorld::getSystems()
{
	return m_systemManager;
}

EntitySystem* EntityWorld::setSystem( SystemType p_type, EntitySystem* p_system,
		bool p_enabled )
{
	return setSystem( (SystemType::SystemTypeIdx)p_type.getIndex(), p_system, p_enabled );
}

EntitySystem* EntityWorld::setSystem( SystemType::SystemTypeIdx p_typeIdx, EntitySystem* p_system,
		bool p_enabled )
{
	p_system->setWorld( this );
	//m_systemsBag.push_back( p_system );
	return m_systemManager->setSystem( p_typeIdx, p_system, p_enabled );
}

EntitySystem* EntityWorld::setSystem( EntitySystem* p_system,
									 bool p_enabled )
{
	return setSystem( p_system->getSystemType(), p_system, p_enabled );
}

void EntityWorld::deleteSystem( SystemType p_type )
{
	deleteSystem( (SystemType::SystemTypeIdx)p_type.getIndex() );
}

void EntityWorld::deleteSystem( SystemType::SystemTypeIdx p_typeIdx )
{
	//deleteSystemFromBag( m_systemManager->getSystem(p_typeIdx) );
	m_systemManager->deleteSystem( p_typeIdx );
}

void EntityWorld::deleteSystem( EntitySystem* system)
{
	//deleteSystemFromBag(system);
	m_systemManager->deleteSystem(system);
}

//void EntityWorld::notifySystems( IPerformer* p_performer, Entity* p_entity )
//{
//	for( unsigned int i = 0; i<m_systemsBag.size(); i++ ) 
//	{
//		p_performer->perform(m_systemsBag[i], p_entity);
//	}
//}

void EntityWorld::notifyManagers( IPerformer* p_performer, Entity* p_entity )
{
	for( unsigned int i=0; i<m_managersBag.size(); i++ )
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

void EntityWorld::check( vector<Entity*>& p_entities, IPerformer* p_performer )
{
	if(!p_entities.empty())
	{
		for ( unsigned int i=0; i<p_entities.size(); i++ )
		{
			Entity* entity = p_entities[i];
			notifyManagers(p_performer, entity);
			m_systemManager->notifySystems(p_performer, entity);
		}
		p_entities.clear();
	}

	// NOTE: This may be a performance issue. Result of direct port of Java impl.
	delete p_performer;
}

void EntityWorld::process()
{
	m_totalGameTime += m_delta;

	prePerformManagers();
	check( m_added,   new AddedPerformer );
	check( m_changed, new ChangedPerformer );
	check( m_disable, new DisabledPerformer );
	check( m_enable,  new EnabledPerformer );
	check( m_deleted, new DeletedPerformer );
	postPerformManagers();

	m_systemManager->updateSynchronous();
}

void EntityWorld::requestToShutDown()
{
	m_shutdown = true;
}

bool EntityWorld::shouldShutDown()
{
	return m_shutdown;
}

void EntityWorld::prePerformManagers()
{
	for( unsigned int i=0; i<m_managersBag.size(); i++ )
		m_managersBag[i]->prePerform();
}

void EntityWorld::postPerformManagers()
{
	for( unsigned int i=0; i<m_managersBag.size(); i++ )
		m_managersBag[i]->postPerform();
}

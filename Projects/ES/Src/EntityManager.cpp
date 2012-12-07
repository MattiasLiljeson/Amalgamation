#include "EntityManager.h"

EntityManager::EntityManager()
{
	m_nextAvailableId = 0;
	m_active = 0;
	m_added = 0;
	m_created = 0;
	m_deleted = 0;
}

EntityManager::~EntityManager()
{

}

Entity* EntityManager::createEntityInstance()
{
	int id;
	if( m_availableIds.empty() ) 
	{
		id = m_nextAvailableId++;
		m_entities.reserve(id+1);
		m_disabled.reserve(id+1);
	}
	else
	{
		id = m_availableIds.top();
		m_availableIds.pop();
	}

	Entity* entity= new Entity(m_world, id);
	m_created++;
	return entity;
}

void EntityManager::added( Entity* p_entity )
{
		m_active++;
		m_added++;

		int idx = p_entity->getIndex();

		if( (int)m_entities.size() <= idx )
		m_entities.resize( idx+1 );

		m_entities[idx] =  p_entity;
}

void EntityManager::enabled( Entity* p_entity )
{
	m_disabled[p_entity->getIndex()] = false;
}

void EntityManager::disabled( Entity* p_entity )
{
	m_disabled[p_entity->getIndex()] = true;
}

void EntityManager::deleted( Entity* p_entity )
{
	m_disabled[p_entity->getIndex()] = false;
	m_availableIds.push(p_entity->getIndex());
	
	// can we delete here? are there still references? 
	delete m_entities[p_entity->getIndex()];
	m_entities[p_entity->getIndex()] = NULL;
	
	m_active--;
	m_deleted++;
}

bool EntityManager::isActive( int p_entityId ) const
{
	return m_entities[p_entityId] != NULL;
}

bool EntityManager::isEnabled( int p_entityId ) const
{
	return !m_disabled[p_entityId];
}

Entity* EntityManager::getEntity( int p_entityId ) const
{
	return m_entities[p_entityId];
}

int EntityManager::getActiveEntityCount() const
{
	return m_active;
}

int EntityManager::getTotalCreated() const
{
	return m_created;
}

int EntityManager::getTotalAdded() const
{
	return m_added; 
}

int EntityManager::getTotalDeleted() const
{
	return m_deleted;
}
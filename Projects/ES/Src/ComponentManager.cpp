#include "ComponentManager.h"


ComponentManager::ComponentManager()
{
}


ComponentManager::~ComponentManager()
{
	for( unsigned int componentTypeIndex=0;
		componentTypeIndex<m_componentsByType.size();
		componentTypeIndex++ )
	{
		for( unsigned int componentIndex=0;
			componentIndex<m_componentsByType[componentTypeIndex].size();
			componentIndex++ )
		{
			if( m_componentsByType[componentTypeIndex][componentIndex] )
			{
				delete m_componentsByType[componentTypeIndex][componentIndex];
			}
		}
	}
}

vector<Component*>& ComponentManager::getComponentsFor(Entity* p_entity,
													   vector<Component*>& p_fillBag)
{
	bitset<ComponentType::NUM_COMPONENT_TYPES> componentBits =
		p_entity->getComponentBits();

	for(unsigned int i=0; i<componentBits.size(); i++ )
	{
		if( componentBits[i] == true)
		{
			p_fillBag.push_back(m_componentsByType[i][p_entity->getIndex()]);
		}
	}
	return p_fillBag;
}

void ComponentManager::deleted( Entity* p_entity )
{
	//removeComponentsOfEntity(p_entity);
	// When an entity is deleted, all components needs to be deleted. The components may
	// not be removed until after all systems have been notified on the "removed" method.
	m_deleted.push_back(p_entity);
}

void ComponentManager::addComponent( Entity* p_entity, ComponentType p_type,
									Component* p_component )
{
	int typeIndex = p_type.getIndex();
	int entityIndex = p_entity->getIndex();

	if( (int)m_componentsByType.size() <= typeIndex )
		m_componentsByType.resize( typeIndex+1 );

	if( (int)m_componentsByType[typeIndex].size() <= entityIndex )
		m_componentsByType[typeIndex].resize( entityIndex+1 );

	m_componentsByType[typeIndex][entityIndex] = p_component;

	p_entity->setComponentBit( typeIndex, true );
}

void ComponentManager::removeComponent( Entity* p_entity, ComponentType p_type )
{
	removeComponent( p_entity, p_type.getIndex());
}

void ComponentManager::removeComponent( Entity* p_entity, ComponentType::ComponentTypeIdx p_typeIdx )
{
	bitset<ComponentType::NUM_COMPONENT_TYPES> bits = p_entity->getComponentBits();
	int entityIndex = p_entity->getIndex();

	if ( bits[p_typeIdx] == true)
	{
		// delete her OK? Any references left?
		m_deleteOnPost.push_back(m_componentsByType[p_typeIdx][p_entity->getIndex()]);

		// HACK: (Johan) Omg, don't forget that you made a change in the matrix, fool!
//		m_componentsByType[p_typeIdx][entityIndex] = NULL;
		m_deleteComponentsByTypeOnPostPerform.push_back(
			pair<int, int>( p_typeIdx, p_entity->getIndex() ));

		p_entity->setComponentBit( p_typeIdx, false );
	}
}

Component* ComponentManager::getComponent( Entity* p_entity, ComponentType p_type )
{
	return getComponent( p_entity, p_type.getIndex() );
}

Component* ComponentManager::getComponent( Entity* p_entity,
										  ComponentType::ComponentTypeIdx p_typeIdx )
{
	return getComponent( p_entity->getIndex(), p_typeIdx );
}

Component* ComponentManager::getComponent( int p_entityIdx, ComponentType p_type )
{
	return getComponent( p_entityIdx, p_type.getIndex() );
}

Component* ComponentManager::getComponent( int p_entityIdx,
										  ComponentType::ComponentTypeIdx p_typeIdx )
{
	if(static_cast<unsigned int>(p_typeIdx) >= m_componentsByType.size())
		return NULL;

	if(m_componentsByType[p_typeIdx].size() > (unsigned int)p_entityIdx)
	{
		return m_componentsByType[p_typeIdx][p_entityIdx];
	}
	return NULL;
}

vector<Component*> ComponentManager::getComponentsByType( ComponentType p_type )
{
	return m_componentsByType[p_type.getIndex()];
}

//void ComponentManager::clean()
//{
//	for (unsigned int i = 0; i < m_deleted.size(); i++)
//		removeComponentsOfEntity(m_deleted[i]);
//	m_deleted.clear();
//}

void ComponentManager::removeComponentsOfEntity( Entity* p_entity )
{
	bitset<ComponentType::NUM_COMPONENT_TYPES> componentBits =
		p_entity->getComponentBits();
	for(unsigned int i=0; i<m_componentsByType.size(); i++ )
	{
		if ((unsigned int)p_entity->getIndex() < m_componentsByType[i].size())
		{
			// Should these be deleted? Alex now says yes. It all depends on when it is
			// called. When an entity is removed, this method is called by postPerform.
			delete m_componentsByType[i][p_entity->getIndex()];
			m_componentsByType[i][p_entity->getIndex()] = NULL;
		}
	}
	componentBits.reset();
	p_entity->setComponentBits(componentBits);
}

void ComponentManager::postPerform()
{
	for(unsigned int i=0; i<m_deleteComponentsByTypeOnPostPerform.size(); i++)
	{
		int typeIndex = m_deleteComponentsByTypeOnPostPerform[i].first;
		int entityIndex = m_deleteComponentsByTypeOnPostPerform[i].second;
		m_componentsByType[typeIndex][entityIndex] = NULL;
	}
	m_deleteComponentsByTypeOnPostPerform.clear();

	// Remove all components here!
	for (unsigned int i = 0; i < m_deleted.size(); i++)
	{
		removeComponentsOfEntity(m_deleted[i]);
	}
	m_deleted.clear();

	// Delete components here!
	for (unsigned int i = 0; i < m_deleteOnPost.size(); i++)
	{
		delete m_deleteOnPost[i];
	}
	m_deleteOnPost.clear();

}

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
	removeComponentsOfEntity(p_entity);
	//m_deleted.push_back(p_entity);
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
	bitset<ComponentType::NUM_COMPONENT_TYPES> bits = p_entity->getComponentBits();
	int typeIndex = p_type.getIndex();
	int entityIndex = p_entity->getIndex();

	if ( bits[typeIndex] == true)
	{
		m_componentsByType[typeIndex][entityIndex] = NULL;
		p_entity->setComponentBit( typeIndex, false );
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
	if(m_componentsByType[p_typeIdx].empty() != true)
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
	for(unsigned int i=0; i<componentBits.size(); i++ )
	{
		if ((unsigned int)p_entity->getIndex() < m_componentsByType[i].size())
		{
			// Should these be deleted?
			//delete m_componentsByType[i][p_entity->getIndex()];
			m_componentsByType[i][p_entity->getIndex()] = NULL;
		}
	}
	componentBits.reset();
	p_entity->setComponentBits(componentBits);
}
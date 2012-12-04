#include "ComponentManager.h"


ComponentManager::ComponentManager()
{
}


ComponentManager::~ComponentManager()
{
}

vector<Component*>& ComponentManager::getComponentsFor(Entity* p_entity, vector<Component*>& p_fillBag)
{
	bitset<ComponentType::NUM_COMPONENT_TYPES> componentBits = p_entity->getComponentBits();

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
	m_deleted.push_back(p_entity);
}

void ComponentManager::addComponent( Entity* p_entity, ComponentType p_type, Component* p_component )
{
	int typeIndex = p_type.getIndex();
	int entityIndex = p_entity->getIndex();

	if( m_componentsByType.size() <= typeIndex )
		m_componentsByType.resize( typeIndex+1 );

	if( m_componentsByType[typeIndex].size() <= entityIndex )
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
	if(m_componentsByType[p_type.getIndex()].empty() != true)
	{
		return m_componentsByType[p_type.getIndex()][p_entity->getIndex()];
	}
	return NULL;
}

vector<Component*> ComponentManager::getComponentsByType( ComponentType p_type )
{
	return m_componentsByType[p_type.getIndex()];
}

void ComponentManager::clean()
{
	for (unsigned int i = 0; i < m_deleted.size(); i++)
		removeComponentsOfEntity(m_deleted[i]);
	m_deleted.clear();
}

void ComponentManager::removeComponentsOfEntity( Entity* p_entity )
{
	bitset<ComponentType::NUM_COMPONENT_TYPES> componentBits = p_entity->getComponentBits();
	for(unsigned int i=0; i<componentBits.size(); i++ )
	{
		// Should these be deleted?
		delete m_componentsByType[i][p_entity->getIndex()];
		m_componentsByType[i][p_entity->getIndex()] = NULL;
	}
	componentBits.reset();
	p_entity->setComponentBits(componentBits);
}
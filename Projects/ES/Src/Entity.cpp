#include "Entity.h"


Entity::Entity( EntityWorld* p_world, int p_index )
{
	 m_world = p_world;
	 m_index = p_index;
}


Entity::~Entity(void)
{
}

void Entity::reset()
{
	m_systemBits.reset();
	m_componentBits.reset();
	//m_UUID = UUID.randomUUID();
}

int Entity::getUUID()
{
	return m_UUID;
}

int Entity::getIndex()
{
	return m_index;
}

EntityWorld* Entity::getWorld()
{
	return m_world;
}

bitset<ComponentType::NUM_COMPONENT_TYPES> Entity::getComponentBits()
{
	return m_componentBits;
}

void Entity::setComponentBits( bitset<ComponentType::NUM_COMPONENT_TYPES> p_componentBits )
{
	m_componentBits = p_componentBits;
}

void Entity::setComponentBit( int p_idx, bool p_value )
{
	m_componentBits[p_idx] = p_value;
}

void Entity::addComponentBit( bitset<ComponentType::NUM_COMPONENT_TYPES> p_componentBits )
{
	m_componentBits |= p_componentBits;
}

void Entity::addComponent( Component* p_component )
{
	if( p_component->getComponentTypeId() != ComponentType::NON_EXISTING )
	{
		addComponent( p_component->getComponentTypeId(), p_component );
	}
	else
	{
		throw "addComponent(): ComponentType not set for this component";
	}
}

void Entity::addComponent( ComponentType::ComponentTypeIdx p_typeIdx, Component* p_component )
{
	addComponent( ComponentType::getTypeFor(p_typeIdx), p_component );
}
void Entity::addComponent( ComponentType p_type, Component* p_component )
{
	m_world->getComponentManager()->addComponent( this, p_type, p_component );
}

void Entity::addTag( ComponentType::ComponentTypeIdx p_typeIdx, Tag* p_tag )
{
	addComponent( p_typeIdx, static_cast<Component*>(p_tag) );
}

void Entity::removeComponent( ComponentType::ComponentTypeIdx p_typeIdx )
{
	removeComponent( ComponentType::getTypeFor(p_typeIdx) );
}

void Entity::removeComponent( ComponentType p_type )
{
	m_world->getComponentManager()->removeComponent( this, p_type );
}

void Entity::applyComponentChanges()
{
	m_world->changedEntity(this);
}

Component* Entity::getComponent( ComponentType::ComponentTypeIdx p_typeIdx )
{
	return m_world->getComponentManager()->getComponent( this, p_typeIdx );
}

bitset<SystemType::NUM_SYSTEM_TYPES> Entity::getSystemBits()
{
	return m_systemBits;
}

void Entity::setSystemBits( bitset<SystemType::NUM_SYSTEM_TYPES> p_systemBits )
{
	m_systemBits = p_systemBits;
}

void Entity::setSystemBit( int p_idx, bool p_value )
{
	m_systemBits[p_idx] = p_value;
}

void Entity::removeComponentBit( bitset<ComponentType::NUM_COMPONENT_TYPES> p_componentBits )
{
	m_componentBits &= ~p_componentBits;
}

void Entity::addSystemBit( bitset<SystemType::NUM_SYSTEM_TYPES> p_systemBits )
{
	m_systemBits |= p_systemBits;
}

void Entity::removeSystemBit( bitset<SystemType::NUM_SYSTEM_TYPES> p_systemBits )
{
	m_systemBits &= ~p_systemBits;
}


bool Entity::isEnabled()
{
	return m_enabled;
}

void Entity::setEnabled( bool p_enabled )
{
	m_enabled = p_enabled;
}
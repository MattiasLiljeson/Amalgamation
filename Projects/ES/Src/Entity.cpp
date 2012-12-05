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

void Entity::addComponentBit( bitset<ComponentType::NUM_COMPONENT_TYPES> p_componentBits )
{
	m_componentBits |= p_componentBits;
}

void Entity::addComponent( ComponentType::ComponentTypeIdx p_typeIdx, Component* p_component )
{
	addComponent( ComponentType::getTypeFor(p_typeIdx), p_component );
}
void Entity::addComponent( ComponentType p_type, Component* p_component )
{
	m_world->getComponentManager()->addComponent( this, p_type, p_component );
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


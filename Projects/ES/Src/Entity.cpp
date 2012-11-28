#include "Entity.h"


Entity::Entity(void)
{
}


Entity::~Entity(void)
{
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
	m_componentBits &= ~p_systemBits;
}


bool Entity::isEnabled()
{
	return m_enabled;
}

void Entity::setEnabled( bool p_enabled )
{
	m_enabled = p_enabled;
}


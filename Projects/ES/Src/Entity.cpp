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

bool Entity::isEnabled()
{
	return m_enabled;
}

void Entity::setEnabled( bool p_enabled )
{
	m_enabled = p_enabled;
}
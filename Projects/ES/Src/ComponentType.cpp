#include "ComponentType.h"

bitset<ComponentType::NUM_COMPONENT_TYPES> ComponentType::s_nextBit = 1;
int ComponentType::s_nextId = 0;
map< ComponentType::ComponentTypeIdx, ComponentType > ComponentType::s_componentTypes;

ComponentType::ComponentType()
{
}


ComponentType::~ComponentType()
{
}

void ComponentType::init( ComponentTypeIdx p_type )
{
	m_bit = s_nextBit;
	s_nextBit = s_nextBit << 1;
	m_id = s_nextId++;

	m_type = p_type;
}

bitset<ComponentType::NUM_COMPONENT_TYPES> ComponentType::getBit()
{
	return m_bit;
}

int ComponentType::getId()
{
	return m_id;
}

ComponentType ComponentType::getTypeFor( ComponentTypeIdx p_component )
{
	ComponentType type;

	if (s_componentTypes.find(p_component) != s_componentTypes.end()) 
	{
		type.init( p_component );
		s_componentTypes[p_component] = type;
	}
	else
	{
		type = s_componentTypes[p_component];
	}

	return type;
}

int ComponentType::getIndexFor( ComponentTypeIdx p_component )
{
	if (s_componentTypes.find(p_component) != s_componentTypes.end())
		return s_componentTypes[p_component].m_id;
	else
		return NON_EXISTING;

}

bitset<ComponentType::NUM_COMPONENT_TYPES> ComponentType::getBitFor( ComponentTypeIdx p_component )
{
	if (s_componentTypes.find(p_component) != s_componentTypes.end())
	{
		return s_componentTypes[p_component].m_bit;
	}
	else
	{
		bitset<NUM_COMPONENT_TYPES> empty;
		return empty;
	}
}

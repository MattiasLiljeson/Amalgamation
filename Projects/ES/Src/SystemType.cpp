#include "SystemType.h"

bitset<SystemType::NUM_SYSTEM_TYPES> SystemType::s_nextBit = 1;
int SystemType::s_nextId = 0;
map< SystemType::SystemTypeIdx, SystemType > SystemType::s_systemTypes;

SystemType::SystemType()
{
}


SystemType::~SystemType()
{
}

void SystemType::init( SystemTypeIdx p_type )
{
	m_bit = 1;
	m_bit = m_bit << p_type;
	m_id = p_type;

	m_type = p_type;
}

bitset<SystemType::NUM_SYSTEM_TYPES> SystemType::getBit()
{
	return m_bit;
}

int SystemType::getId()
{
	return m_id;
}

SystemType SystemType::getTypeFor( SystemTypeIdx p_system )
{
	SystemType type;

	if (s_systemTypes.find(p_system) != s_systemTypes.end()) 
	{
		type = s_systemTypes[p_system];
	}
	else
	{
		type.init( p_system );
		s_systemTypes[p_system] = type;
	}

	return type;
}

int SystemType::getIndexFor( SystemTypeIdx p_system )
{
	if (s_systemTypes.find(p_system) != s_systemTypes.end())
		return s_systemTypes[p_system].m_id;
	else
		return NON_EXISTING;

}

bitset<SystemType::NUM_SYSTEM_TYPES> SystemType::getBitFor( SystemTypeIdx p_system )
{
	if (s_systemTypes.find(p_system) != s_systemTypes.end())
	{
		return s_systemTypes[p_system].m_bit;
	}
	else
	{
		bitset<NUM_SYSTEM_TYPES> empty;
		return empty;
	}
}

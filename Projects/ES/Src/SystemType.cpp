#include "SystemType.h"

bitset SystemType::s_nextBit = 1;
int SystemType::s_nextId = 0;
map< SystemType::SystemType, SystemType > SystemType::s_SystemTypes;

SystemType::SystemType()
{
}


SystemType::~SystemType()
{
}

void SystemType::init( SystemTypeID p_type )
{
	m_bit = s_nextBit;
	s_nextBit = s_nextBit << 1;
	m_id = s_nextId++;

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

SystemType SystemType::getTypeFor( SystemTypeID p_system )
{
	SystemType type;

	if (s_SystemTypes.find(p_system) != s_SystemTypes.end()) 
	{
		type.init( p_system );
		s_SystemTypes[p_system] = type;
	}
	else
	{
		type = s_SystemTypes[p_system];
	}

	return type;
}

int SystemType::getIndexFor( SystemTypeID p_system )
{
	if (s_SystemTypes.find(p_system) != s_SystemTypes.end())
		return s_SystemTypes[p_system].m_id;
	else
		return NON_EXISTING;

}

bitset<SystemType::NUM_SYSTEM_TYPES> SystemType::getBitFor( SystemTypeID p_system )
{
	if (s_SystemTypes.find(p_system) != s_SystemTypes.end())
	{
		return s_SystemTypes[p_system].m_bit;
	}
	else
	{
		bitset<NUM_SYSTEM_TYPES> empty;
		return empty;
	}
}

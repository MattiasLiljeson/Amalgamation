#pragma once

#include <bitset>
#include <map>

using namespace std;

class SystemType
{
public:	//Enums
	enum SystemTypeIdx
	{
		NON_EXISTING = -1,
		NUM_SYSTEM_TYPES
	};

public:
	SystemType(void);
	~SystemType(void);

	static SystemType getTypeFor( SystemTypeIdx p_system );
	static int getIndexFor( SystemTypeIdx p_system );
	static bitset<NUM_SYSTEM_TYPES> getBitFor( SystemTypeIdx p_system );

	bitset<NUM_SYSTEM_TYPES> getBit();
	int getId();

private:
	void init( SystemTypeIdx p_type );

private:
	static bitset<NUM_SYSTEM_TYPES> s_nextBit;
	static int s_nextId;
	static map<SystemTypeIdx, SystemType> s_systemTypes;

	bitset<NUM_SYSTEM_TYPES> m_bit;
	int m_id;
	SystemTypeIdx m_type;
};


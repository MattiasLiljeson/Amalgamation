#pragma once

#include <bitset>

using namespace std;

class SystemType
{
public:	//Enums
	enum SystemTypeID
	{
		NON_EXISTING,
		NUM_SYSTEM_TYPES
	};

public:
	SystemType(void);
	~SystemType(void);

	static SystemType getTypeFor( SystemTypeID p_system );
	static int getIndexFor( SystemTypeID p_system );
	static bitset<NUM_SYSTEM_TYPES> getBitFor( SystemTypeID p_system );

	bitset<NUM_SYSTEM_TYPES> getBit();
	int getId();

private:
	void init( SystemTypeID p_type );

private:
	static bitset<NUM_SYSTEM_TYPES> s_nextBit;
	static int s_nextId;
	static map<SystemTypeID, ComponentType> s_systemTypes;

	bitset<NUM_SYSTEM_TYPES> m_bit;
	int m_id;
	SystemTypeID m_type;
};


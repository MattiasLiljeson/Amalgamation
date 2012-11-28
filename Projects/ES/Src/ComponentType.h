#pragma once

#include <bitset>
#include <map>

using namespace std;

class ComponentType
{
public:	//Enums
	enum ComponentTypeID
	{
		NON_EXISTING,
		NUM_COMPONENT_TYPES
	};

public:
	ComponentType();
	~ComponentType();

	static ComponentType getTypeFor( ComponentTypeID p_component );
	static int getIndexFor( ComponentTypeID p_component );
	static bitset<NUM_COMPONENT_TYPES> getBitFor( ComponentTypeID p_component );

	bitset<NUM_COMPONENT_TYPES> getBit();
	int getId();

private:
	void init( ComponentTypeID p_type );

private:
	static bitset<NUM_COMPONENT_TYPES> s_nextBit;
	static int s_nextId;
	static map< ComponentTypeID, ComponentType > s_componentTypes;

	bitset<NUM_COMPONENT_TYPES> m_bit;
	int m_id;
	ComponentTypeID m_type;

};


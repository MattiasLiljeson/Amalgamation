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
		EntitySystem,
		TestSystem,
		TestSystem2,
		PrintPositionsSystem,
		NetworkListenerSystem,
		InputSystem,
		PhysicsSystem,
		NUM_SYSTEM_TYPES
	};

public:
	/**
	 * The constructor is not private but should not be used for creating an instance for
	 * this class. This method is public so that the class can be allocated on the stack.
	 * Use getTypeFor() if you want an instance of this class.
	 */
	SystemType();
	~SystemType();

	static SystemType getTypeFor( SystemTypeIdx p_system );
	static int getIndexFor( SystemTypeIdx p_system );
	static bitset<NUM_SYSTEM_TYPES> getBitFor( SystemTypeIdx p_system );

	bitset<NUM_SYSTEM_TYPES> getBit();
	int getIndex();

private:

	/**
	 * This method is private. Initialization is done through getTypeFor(). Use that if
	 * you want an instance of this class. This method will initialize the type with a
	 * correct bit and correct index.
	 * @param p_type Type to be initialized to.
	 */
	void init( SystemTypeIdx p_type );

private:
	static bitset<NUM_SYSTEM_TYPES> s_nextBit;
	static int s_nextId;
	static map<SystemTypeIdx, SystemType> s_systemTypes;

	bitset<NUM_SYSTEM_TYPES> m_bit;
	SystemTypeIdx m_idx;
};


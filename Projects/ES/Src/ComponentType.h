#pragma once

#include <bitset>
#include <map>

using namespace std;

class ComponentType
{
public:	//Enums

	// This enum is currently used for statically mapping Component types,
	// and if it is changed the tests will fail. In the future this
	// functionality will be made dynamically and any testing issues will go away.
	// Also the same thing goes for SystemType.
	enum ComponentTypeIdx
	{
		NON_EXISTING = -1,
		RenderInfo,
		Input,
		NetworkSynced,
		PhysicsBody,
		PhysUnknown,
		CameraInfo,
		AudioComponent,
		AudioListener,
		ShipController,
		Impulse,
		BodyInitData,
		LookAtEntity,
		MainCamera,
		PlayerScore,
		MenuItem,
		HudElement,
				   // *FUGLY HACK!*
		Transform, // There is a bug when an unused component idx is at the last position
				   // in the enum. Therefore, put transform at the end for now. :/
		ShipModule,
		ConnectionPointSet,
		SpeedBoosterModule,
		MinigunModule,
		ShieldModule,
		MineLayerModule,
		StandardMine,
		NUM_COMPONENT_TYPES
	};

public:
	ComponentType();
	~ComponentType();

	static ComponentType getTypeFor( ComponentTypeIdx p_component );
	static ComponentTypeIdx getIndexFor( ComponentTypeIdx p_component );
	static bitset<NUM_COMPONENT_TYPES> getBitFor( ComponentTypeIdx p_component );

	bitset<NUM_COMPONENT_TYPES> getBit();
	ComponentTypeIdx getIndex();

private:
	void init( ComponentTypeIdx p_type );

private:
	static bitset<NUM_COMPONENT_TYPES> s_nextBit;
	static int s_nextId;
	static map< ComponentTypeIdx, ComponentType > s_componentTypes;

	bitset<NUM_COMPONENT_TYPES> m_bit;
	ComponentTypeIdx m_idx;
	ComponentTypeIdx m_type;

};


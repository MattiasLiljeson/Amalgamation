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
		// -------------------
		// Regular Components
		// -------------------
		RenderInfo,
		Input,
		NetworkSynced,
		ClientInfo,
		Extrapolate,
		InterpolationComponent,
		PhysicsBody,
		PhysUnknown,
		CameraInfo,
		AudioInfo,
		AudioListener,
		PositionalSoundSource,
		ShipFlyController,
		ShipEditController,
		Impulse,
		BodyInitData,
		BoundingVolumeInitData,
		LookAtEntity,
		PlayerCameraController,
		PlayerScore,
		MenuItem,
		HudElement,
		EntityParent,
		LoadMesh,
		Transform,
		ShipModule,
		ConnectionPointSet,
		ShipConnectionPointHighlights,
		SpawnPointSet,
		SpeedBoosterModule,
		MinigunModule,
		ShieldModule,
		MineLayerModule,
		StandardMine,
		StandardRocket,
		RocketLauncherModule,
		Connector1to2Module,
		LightSources,
		LightBlinker,
		Vibration,
		ParticleSystemsComponent,
		PickComponent,
		ParticleSystemEmitter,
		ParticleSystemServerComponent,
		DebugMove,
		PlayerState,

		// -------------------
		// Tags									(Replace with dedicated ES-tag management?)
		// -------------------
		Tag,
		TAG_ShipFlyMode,		
		TAG_ShipEditMode,
		TAG_LookAtFollowMode,
		TAG_LookAtOrbitMode,
		TAG_Ship,
		TAG_MyShip,
		TAG_MainCamera,
		TAG_ShadowCamera,
		// -------------------
	
		/************************************************************************/
		/* Things that should be sent to all clients but its not synced			*/
		/* afterwards.															*/
		/************************************************************************/
		StaticProp,
		LightsComponent,
		MeshOffsetTransform,
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


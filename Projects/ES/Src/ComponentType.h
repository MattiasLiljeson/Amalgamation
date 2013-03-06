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
		AnomalyBomb,
		Input,
		NetworkSynced,
		ClientInfo,
		ColorTone,
		CircularMovement,
		AxisRotate,
		Extrapolate,
		InterpolationComponent,
		PhysicsBody,
		PhysUnknown,
		DestroyOnParticlesDeath,
		CameraInfo,
		AudioInfo,
		AudioListener,
		SoundComponent,
		ShipFlyController,
		ShipEditController,
		Impulse,
		BodyInitData,
		ModuleStatusVisualizationMode,
		BoundingVolumeInitData,
		LookAtEntity,
		PlayerCameraController,
		PlayerComponent,
		MenuItem,
		HudElement,
		EntityParent,
		LoadMesh,
		Transform,
		ShipModule,
		EnvironmentValues,
		ConnectionPointSet,
		ShipConnectionPointHighlights,
		SpawnPointSet,
		SpeedBoosterModule,
		MinigunModule,
		ShieldModule,
		ShieldPlate,
		MineLayerModule,
		StandardMine,
		StandardRocket,
		RocketLauncherModule,
		Connector1to2Module,
		EmpModule,
		FlareGunModule,
		GravityBombModule,
		TeslaModule,
		LightSources,
		LightBlinker,
		Vibration,
		ParticleSystemsComponent,
		PickComponent,
		ParticleSystemEmitter_DEPRECATED,
		ParticleSystemServerComponent,
		DebugMove,
		uvRectCrop,
		PlayerState,
		LevelPieceRoot,
		LevelInfo,
		GameState,
		Gradient,
		SpeedBuffer,
		AnomalyAcceleratorModule,
		TeslaCoilModule,
		EmpRocketLauncherModule,
		InterpolationComponent2,
		DamageComponent,
		ModuleOnChamberSpawnPoint,

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
		TAG_LookAtMainCamera,
		TAG_Highlight,
		TAG_PositionalSound,
		TAG_AmbientRangeSound,
		// -------------------
	
		/************************************************************************/
		/* Things that should be sent to all clients but its not synced			*/
		/* afterwards.															*/
		/************************************************************************/
		StaticProp,
		ServerGameState,
		LightsComponent,
		MeshOffsetTransform,
		SkeletalAnimation,
		BoundingSphere,
		BoundingBox,
		SelectionMarker,
		Sprite,
		SlotMarker,
		SlotMarkerSprite,
		AnomalyBombEffectPiece,
		TeslaEffectPiece,
		ShipHighlight,
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


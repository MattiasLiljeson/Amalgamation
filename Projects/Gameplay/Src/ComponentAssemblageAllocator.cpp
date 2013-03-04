#include "ComponentAssemblageAllocator.h"

// Components
#include "AnomalyAcceleratorModule.h"
#include "AnomalyBomb.h"
#include "AudioInfo.h"
#include "AudioListener.h"
#include "BodyInitData.h"
#include "ConnectionPointSet.h"
#include "Connector1to2Module.h"
#include "DamageComponent.h"
#include "DestroyOnParticlesDeath.h"
#include "EmpModule.h"
#include "EntityParent.h"
#include "EnvironmentValues.h"
#include "FlareGunModule.h"
#include "GameplayTags.h"
#include "GravityBombModule.h"
#include "HudElement.h"
#include "LevelInfo.h"
#include "LevelPieceRoot.h"
#include "LightBlinker.h"
#include "LightsComponent.h"
#include "LoadMesh.h"
#include "MineLayerModule.h"
#include "MinigunModule.h"
#include "MinigunModule.h"
#include "PhysicsBody.h"
#include "PlayerCameraController.h"
#include "PlayerCameraController.h"
#include "RenderInfo.h"
#include "RocketLauncherModule.h"
#include "ShieldModule.h"
#include "ShipEditController.h"
#include "ShipFlyController.h"
#include "ShipModule.h"
#include "SkeletalAnimation.h"
#include "SpeedBoosterModule.h"
#include "SpeedBuffer.h"
#include "TeslaModule.h"
#include "Transform.h"
#include "Vibration.h"

// unsorted automatically added includes. Sort them when added!
#include <Component.h>

ComponentAssemblageAllocator::ComponentAssemblageAllocator()
{
	handle<AudioInfo>();
	handle<AudioListener>();
	handle<BodyInitData>();
	handle<ConnectionPointSet>();
	handle<HudElement>();
	handle<MinigunModule>();
	handle<PhysicsBody>();
	handle<PlayerCameraController>();
	handle<RenderInfo>();
	handle<ShipEditController>();
	handle<ShipFlyController>();
	handle<ShipModule>();
	handle<SpeedBoosterModule>();
	handle<MinigunModule>();
	handle<PlayerCameraController>();
	handle<ShieldModule>();
	handle<Vibration>();
	handle<MineLayerModule>();
	handle<RocketLauncherModule>();
	handle<Connector1to2Module>();
	handle<Transform>();
	handle<EntityParent>();
	handle<LoadMesh>();
	handle<LightsComponent>();
	handle<LightBlinker>();
	handle<LevelPieceRoot>();
	handle<LevelInfo>();
	handle<EnvironmentValues>();
	handle<SpeedBuffer>();
	handle<DestroyOnParticlesDeath>();
	handle<SkeletalAnimation>();
	handle<AnomalyAcceleratorModule>();
	handle<TeslaModule>();
	handle<FlareGunModule>();
	handle<GravityBombModule>();
	handle<EmpModule>();
	handle<AnomalyBomb>();
	handle<DamageComponent>();
}
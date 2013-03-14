#include "ComponentAssemblageAllocator.h"

// Components (Keep it sorted)
#include "AnomalyAcceleratorModule.h"
#include "AnomalyBomb.h"
#include "AudioInfo.h"
#include "AudioListener.h"
#include "AxisRotate.h"
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
#include "HudElement.h"
#include "LevelInfo.h"
#include "LevelPieceRoot.h"
#include "LightBlinker.h"
#include "LightsComponent.h"
#include "LoadMesh.h"
#include "MineLayerModule.h"
#include "MinigunModule.h"
#include "PhysicsBody.h"
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
#include "TeslaCoilEffect.h"
#include "TeslaCoilModule.h"
#include "TeslaEffectPiece.h"
#include "Transform.h"
#include "Vibration.h"

// unsorted automatically added includes. Sort them when added!
#include <Component.h>

ComponentAssemblageAllocator::ComponentAssemblageAllocator()
{
	// Instantiations of components (Keep it sorted)
	handle<AnomalyAcceleratorModule>();
	handle<AnomalyBomb>();
	handle<AudioInfo>();
	handle<AudioListener>();
	handle<AxisRotate>();
	handle<BodyInitData>();
	handle<ConnectionPointSet>();
	handle<Connector1to2Module>();
	handle<DamageComponent>();
	handle<DestroyOnParticlesDeath>();
	handle<EmpModule>();
	handle<EntityParent>();
	handle<EnvironmentValues>();
	handle<FlareGunModule>();
	handle<HudElement>();
	handle<LevelInfo>();
	handle<LevelPieceRoot>();
	handle<LightBlinker>();
	handle<LightsComponent>();
	handle<LoadMesh>();
	handle<MineLayerModule>();
	handle<MinigunModule>();
	handle<PhysicsBody>();
	handle<PlayerCameraController>();
	handle<RenderInfo>();
	handle<RocketLauncherModule>();
	handle<ShieldModule>();
	handle<ShipEditController>();
	handle<ShipFlyController>();
	handle<ShipModule>();
	handle<SkeletalAnimation>();
	handle<SpeedBoosterModule>();
	handle<SpeedBuffer>();
	handle<TeslaCoilEffect>();
	handle<TeslaCoilModule>();
	handle<TeslaEffectPiece>();
	handle<Transform>();
	handle<Vibration>();
}
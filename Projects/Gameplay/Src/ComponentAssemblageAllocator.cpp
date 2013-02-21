#include "ComponentAssemblageAllocator.h"

// Components
#include <Component.h>
#include "AudioInfo.h"
#include "AudioListener.h"
#include "BodyInitData.h"
#include "ConnectionPointSet.h"
#include "HudElement.h"
#include "MinigunModule.h"
#include "PhysicsBody.h"
#include "PlayerCameraController.h"
#include "RenderInfo.h"
#include "ShipEditController.h"
#include "ShipFlyController.h"
#include "ShipModule.h"
#include "SpeedBoosterModule.h"
#include "MinigunModule.h"
#include "GameplayTags.h"
#include "PlayerCameraController.h"
#include "ShieldModule.h"
#include "MineLayerModule.h"
#include "RocketLauncherModule.h"
#include "Connector1to2Module.h"
#include "Transform.h"
#include "Vibration.h"
#include "EntityParent.h"
#include "LoadMesh.h"
#include "LightsComponent.h"
#include "LightBlinker.h"
#include "LevelPieceRoot.h"
#include "LevelInfo.h"
#include "EnvironmentValues.h"
#include "DestroyOnParticlesDeath.h"

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
	handle<DestroyOnParticlesDeath>();
}
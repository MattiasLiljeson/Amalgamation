#include "ShipEditModeSystem.h"

#include <TcpClient.h>
#include <AglQuaternion.h>
#include "InputBackendSystem.h"
#include "PhysicsSystem.h"
#include "Transform.h"
#include "ShipEditController.h"
#include "PacketType.h"
#include "NetworkType.h"
#include "NetworkSynced.h"
#include "Control.h"
#include "AntTweakBarWrapper.h"
#include "PhysicsBody.h"

#include "GameplayTags.h"

ShipEditControllerSystem::ShipEditControllerSystem( InputBackendSystem* p_inputBackend, 
									   PhysicsSystem* p_physicsSystem/*, 
									   TcpClient* p_client */ ) : 
					EntitySystem( SystemType::ShipEditControllerSystem, 3,
								  ComponentType::ComponentTypeIdx::ShipEditController,
								  ComponentType::ComponentTypeIdx::TAG_ShipEditMode,
								  ComponentType::ComponentTypeIdx::Transform)

{

}

ShipEditControllerSystem::~ShipEditControllerSystem()
{

}

void ShipEditControllerSystem::initialize()
{

}

void ShipEditControllerSystem::processEntities( const vector<Entity*>& p_entities )
{

}

#include "ShipEditControllerSystem.h"
#include "ShipEditController.h"

#include <TcpClient.h>
#include <AglQuaternion.h>
#include "InputBackendSystem.h"
#include "PhysicsSystem.h"
#include "Transform.h"
#include "PacketType.h"
#include "NetworkType.h"
#include "NetworkSynced.h"
#include "Control.h"
#include "AntTweakBarWrapper.h"
#include "PhysicsBody.h"
#include "ShipInputProcessingSystem.h"

#include "GameplayTags.h"

ShipEditControllerSystem::ShipEditControllerSystem( ShipInputProcessingSystem* p_shipInput, 
									   PhysicsSystem* p_physicsSystem/*, 
									   TcpClient* p_client */ ) : 
					EntitySystem( SystemType::ShipEditControllerSystem, 3,
								  ComponentType::ComponentTypeIdx::ShipEditController,
								  ComponentType::ComponentTypeIdx::TAG_ShipEditMode,
								  ComponentType::ComponentTypeIdx::Transform)

{
	m_shipInput = p_shipInput;
	m_physics = p_physicsSystem;
	// m_client = p_client;
}

ShipEditControllerSystem::~ShipEditControllerSystem()
{

}

void ShipEditControllerSystem::initialize()
{

}

void ShipEditControllerSystem::processEntities( const vector<Entity*>& p_entities )
{
	if (p_entities.size()>0)
	{
		float dt = m_world->getDelta();
		// Fetch the status of the various input methods.
		ShipInputProcessingSystem::ResultingInputForces input = m_shipInput->getProcessedInput();

		for(unsigned int i=0; i<p_entities.size(); i++ )
		{
			Entity* ship = p_entities[i];
			ShipEditController* controller = static_cast<ShipEditController*>(
				ship->getComponent( ComponentType::ComponentTypeIdx::ShipFlyController ) );


			// State switch handling
			if (input.stateSwitchInput>0.0f)
			{
				ship->removeComponent(ComponentType::TAG_ShipEditMode); // Disable this state...
				ship->addTag(ComponentType::TAG_ShipFlyMode, new ShipFlyMode_TAG());  // ...and switch to fly state.
				ship->applyComponentChanges();
			}
		}
	}
}

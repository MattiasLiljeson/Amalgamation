#include "PlayerCameraControllerSystem.h"

#include "ShipInputProcessingSystem.h"

#include "GameplayTags.h"
#include "PlayerCameraController.h"

PlayerCameraControllerSystem::PlayerCameraControllerSystem( ShipInputProcessingSystem* p_shipInput) : 
					EntitySystem( SystemType::PlayerCameraControllerSystem, 1,
								  ComponentType::ComponentTypeIdx::PlayerCameraController)

{
	m_shipInput = p_shipInput;
}

PlayerCameraControllerSystem::~PlayerCameraControllerSystem()
{

}

void PlayerCameraControllerSystem::initialize()
{

}

void PlayerCameraControllerSystem::processEntities( const vector<Entity*>& p_entities )
{
	if (p_entities.size()>0)
	{
		float dt = m_world->getDelta();
		// Fetch the status of the various input methods.
		ShipInputProcessingSystem::ResultingInputForces input = m_shipInput->getProcessedInput();

		for(unsigned int i=0; i<p_entities.size(); i++ )
		{
			/* This only works when camera not handled by server, rewrite this like shipflycontroller
			Entity* ship = p_entities[i];
			PlayerCameraController* controller = static_cast<PlayerCameraController*>(
				ship->getComponent( ComponentType::ComponentTypeIdx::PlayerCameraController ) );

			// get lookAt tags if they exist
			LookAtFollowMode_TAG* lookAtFollow=NULL;
			LookAtOrbitMode_TAG* lookAtOrbit=NULL;

			Component* t = p_entities[i]->getComponent( ComponentType::ComponentTypeIdx::TAG_LookAtFollowMode );
			if (t!=NULL)
				lookAtFollow = static_cast<LookAtFollowMode_TAG*>(t);


			t = p_entities[i]->getComponent( ComponentType::ComponentTypeIdx::TAG_LookAtOrbitMode );
			if (t!=NULL)
				lookAtOrbit = static_cast<LookAtOrbitMode_TAG*>(t);

			// State switch handling
			if (input.stateSwitchInput)
			{
				if (lookAtFollow)
				{
					ship->removeComponent(ComponentType::TAG_LookAtFollowMode); // Disable this state...
					ship->addTag(ComponentType::TAG_LookAtOrbitMode, new LookAtOrbitMode_TAG());  // ...and switch to orbit state.
					ship->applyComponentChanges();
				}
				else if (lookAtOrbit)
				{
					ship->removeComponent(ComponentType::TAG_LookAtOrbitMode); // Disable this state...
					ship->addTag(ComponentType::TAG_LookAtFollowMode, new LookAtFollowMode_TAG());  // ...and switch to follow state.
					ship->applyComponentChanges();
				}
			}
			*/
		}
	}
}

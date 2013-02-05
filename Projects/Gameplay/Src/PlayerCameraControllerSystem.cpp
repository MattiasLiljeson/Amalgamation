#include "PlayerCameraControllerSystem.h"
#include "AglVector3.h"
#include "ShipInputProcessingSystem.h"

#include "GameplayTags.h"
#include "PlayerCameraController.h"
#include "CameraControlPacket.h"
#include "TimerSystem.h"
#include "NetworkSynced.h"

PlayerCameraControllerSystem::PlayerCameraControllerSystem( ShipInputProcessingSystem* p_shipInput,
														   TcpClient* p_client ) : 
					EntitySystem( SystemType::PlayerCameraControllerSystem, 1,
								  ComponentType::ComponentTypeIdx::PlayerCameraController)

{
	m_shipInput = p_shipInput;
	m_client=p_client;
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

			// packet handling
			if(static_cast<TimerSystem*>(m_world->getSystem(SystemType::TimerSystem))->
				checkTimeInterval(TimerIntervals::Every8Millisecond))
			{
				int stateForPacket = CameraControlPacket::steeringState;
				if (lookAtOrbit) stateForPacket=CameraControlPacket::editState;
				/************************************************************************/
				/* Send the control packet to the server!								*/
				/************************************************************************/
				NetworkSynced* netSync = static_cast<NetworkSynced*>(ship->getComponent(
					ComponentType::NetworkSynced));

				sendCameraControllerPacketToServer(netSync,AglVector3::up(),stateForPacket);
			}
		}
	}
}

void PlayerCameraControllerSystem::sendCameraControllerPacketToServer( NetworkSynced* p_syncedInfo, 
																	  AglVector3& p_movement, int p_state )
{
	CameraControlPacket cameraPacket;
	cameraPacket.entityId = p_syncedInfo->getNetworkIdentity();
	cameraPacket.movement = p_movement;
	cameraPacket.state = p_state;

	m_client->sendPacket( cameraPacket.pack() );
}

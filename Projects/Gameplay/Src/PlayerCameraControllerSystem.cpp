#include "PlayerCameraControllerSystem.h"
#include "AglVector3.h"
#include "ShipInputProcessingSystem.h"

#include "GameplayTags.h"
#include "PlayerCameraController.h"
#include "PlayerState.h"
#include "CameraControlPacket.h"
#include "TimerSystem.h"
#include "NetworkSynced.h"
#include "TcpClient.h"
#include "ClientStateSystem.h"

PlayerCameraControllerSystem::PlayerCameraControllerSystem( ShipInputProcessingSystem* p_shipInput,
														   TcpClient* p_client ) : 
					EntitySystem( SystemType::PlayerCameraControllerSystem, 1,
								  ComponentType::ComponentTypeIdx::PlayerCameraController,
								  ComponentType::ComponentTypeIdx::PlayerState)

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

	ClientStateSystem* stateSystem = static_cast<ClientStateSystem*>(
		m_world->getSystem(SystemType::ClientStateSystem));

	if(stateSystem->getCurrentState() == GameStates::INGAME){
		if (p_entities.size()>0)
		{
			float dt = m_world->getDelta();
			// Fetch the status of the various input methods.
			ShipInputProcessingSystem::ResultingInputForces input = 
				m_shipInput->getProcessedInput();

			for(unsigned int i=0; i<p_entities.size(); i++ )
			{
				Entity* ship = p_entities[i];
				PlayerCameraController* controller = static_cast<PlayerCameraController*>(
					ship->getComponent( ComponentType::ComponentTypeIdx::PlayerCameraController ) );
				PlayerState* state = static_cast<PlayerState*>(
					ship->getComponent( ComponentType::ComponentTypeIdx::PlayerState ) );

				// Get current input
				AglVector3 inputMovement(input.editMoveVerticalInput,
					-input.editMoveHorizontalInput,0.0f);

				// accumulate movement for transfer
				controller->accumulatedCameraMovement += inputMovement;

				// State switch handling
				if (input.stateSwitchInput)
				{
					if (state->state==PlayerStates::steeringState)
						state->state=PlayerStates::editState;
					else
						state->state=PlayerStates::steeringState;
				}

				// packet handling
				/*
				if(static_cast<TimerSystem*>(m_world->getSystem(SystemType::TimerSystem))->
					checkTimeInterval(TimerIntervals::Every8Millisecond))
				{
					if (controller->accumulatedCameraMovement.length()>0.0f &&
						controller->accumulatedCameraMovement.length()<1.0f)
						AglVector3::normalize(controller->accumulatedCameraMovement);
					NetworkSynced* netSync = static_cast<NetworkSynced*>(ship->getComponent(
						ComponentType::NetworkSynced));

					sendCameraControllerPacketToServer(netSync,
						controller->accumulatedCameraMovement,state->state);

					controller->accumulatedCameraMovement = AglVector3();
				}
				*/
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

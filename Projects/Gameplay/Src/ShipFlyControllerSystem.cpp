#include "ShipFlyControllerSystem.h"

#include <TcpClient.h>
#include <AglQuaternion.h>
#include "InputBackendSystem.h"
#include "PhysicsSystem.h"
#include "Transform.h"
#include "ShipFlyController.h"
#include "PacketType.h"
#include "EntityType.h"
#include "NetworkSynced.h"
#include "Control.h"
#include "AntTweakBarWrapper.h"
#include "PhysicsBody.h"
#include "ShipInputProcessingSystem.h"
#include "TimerSystem.h"

#include "GameplayTags.h"
#include "ConnectionPointSet.h"
#include "SpeedBoosterModule.h"
#include "ThrustPacket.h"
#include "EntityUpdatePacket.h"
#include "MeshOffsetTransform.h"

#define FORCE_VS_OUTPUT

ShipFlyControllerSystem::ShipFlyControllerSystem( ShipInputProcessingSystem* p_shipInput,
										    PhysicsSystem* p_physicsSystem,
											TcpClient* p_client ) : 
					  EntitySystem( SystemType::ShipFlyControllerSystem, 3,
									ComponentType::ComponentTypeIdx::ShipFlyController,
									ComponentType::ComponentTypeIdx::Transform,
									ComponentType::ComponentTypeIdx::TAG_ShipFlyMode)
{
	m_shipInput = p_shipInput;
	m_physics = p_physicsSystem;
	m_client = p_client;
}

ShipFlyControllerSystem::~ShipFlyControllerSystem()
{
}


void ShipFlyControllerSystem::initialize()
{
}

void ShipFlyControllerSystem::processEntities( const vector<Entity*>& p_entities )
{
	if (p_entities.size()>0)
	{
		float dt = m_world->getDelta();
		
		// Fetch the status of the various input methods.
		ShipInputProcessingSystem::ResultingInputForces input = m_shipInput->getProcessedInput();

		for(unsigned int i=0; i<p_entities.size(); i++ )
		{

			// Calculate impulse data to send to server
			Entity* ship = p_entities[i];
			ShipFlyController* controller = static_cast<ShipFlyController*>(
				ship->getComponent( ComponentType::ComponentTypeIdx::ShipFlyController ) );

			Transform* transform = static_cast<Transform*>(
				ship->getComponent( ComponentType::ComponentTypeIdx::Transform ) );

			MeshOffsetTransform* meshOffset = static_cast<MeshOffsetTransform*>(
				ship->getComponent( ComponentType::ComponentTypeIdx::MeshOffsetTransform ) );

			if (meshOffset)
			{
				AglMatrix mat = meshOffset->offset;
				transform = new Transform(mat.transpose() * transform->getMatrix());
			}
			else
			{
				transform = new Transform(transform->getMatrix());
			}

			// Calc rotation from player input
			AglVector3 inputAngles(input.verticalInput,input.horizontalInput,input.rollInput);

			// Turning multiplier
			float  turnSpeed = controller->getTurningSpeed() * dt;
			// Thrust multiplier
			float  thrustPower = controller->getThrustPower() * dt;

			// Calc translation from player input
			AglVector3 thrustVec;
			thrustVec += transform->getMatrix().GetForward()	* input.thrustInput 
				* thrustPower;
			thrustVec += transform->getMatrix().GetRight()		* input.strafeHorizontalInput 
				* thrustPower;
			thrustVec += transform->getMatrix().GetUp()			* input.strafeVerticalInput 
				* thrustPower;

			// Calc rotation from player input
			AglVector3 angularVec=inputAngles*turnSpeed;
			AglQuaternion quat = transform->getRotation();
			quat.transformVector(angularVec);


			controller->m_thrustPowerAccumulator += thrustVec;
			controller->m_turnPowerAccumulator += angularVec;

			// Handle switch to edit mode
			if (input.stateSwitchInput != 0)
			{
				controller->m_thrustPowerAccumulator = AglVector3::zero();
				controller->m_turnPowerAccumulator = AglVector3::zero();
				ship->removeComponent(ComponentType::TAG_ShipFlyMode); // Disable this state...
				ship->addTag(ComponentType::TAG_ShipEditMode, new ShipEditMode_TAG()); // ...and switch to edit state.
				ship->applyComponentChanges();
			}

			// Send data to server
			if(static_cast<TimerSystem*>(m_world->getSystem(SystemType::TimerSystem))->
				checkTimeInterval(TimerIntervals::Every8Millisecond))
			{
				/************************************************************************/
				/* Send the thrust packet to the server!								*/
				/************************************************************************/

				NetworkSynced* netSync = static_cast<NetworkSynced*>(ship->getComponent(
					ComponentType::NetworkSynced));
				sendThrustPacketToServer(netSync,
					controller->m_thrustPowerAccumulator, 
					controller->m_turnPowerAccumulator);

				controller->m_thrustPowerAccumulator = AglVector3::zero();
				controller->m_turnPowerAccumulator = AglVector3::zero();
			}

			// Handle data sent to us from server
			// handleTransformInterpolation( controller, transform );

			delete transform;
		}
	}
}

// NOTE: (Johan) 1/2-13 Removed the whole getSpeedBoost method on client side, since
// it wasn't used, and that was quite confusing.

void ShipFlyControllerSystem::sendThrustPacketToServer(NetworkSynced* p_syncedInfo, 
													   AglVector3 p_thrust, 
													   AglVector3 p_angularVec)
{
	ThrustPacket thrustPacket;
	thrustPacket.entityId = p_syncedInfo->getNetworkIdentity();
	thrustPacket.thrustVector = p_thrust;
	thrustPacket.angularVector = p_angularVec;

	m_client->sendPacket( thrustPacket.pack() );
}

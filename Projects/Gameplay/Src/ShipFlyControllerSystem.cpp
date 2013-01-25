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
			Entity* ship = p_entities[i];
			ShipFlyController* controller = static_cast<ShipFlyController*>(
				ship->getComponent( ComponentType::ComponentTypeIdx::ShipFlyController ) );

			Transform* transform = static_cast<Transform*>(
				ship->getComponent( ComponentType::ComponentTypeIdx::Transform ) );

			// Calc rotation from player input
			AglVector3 inputAngles(input.verticalInput,input.horizontalInput,input.rollInput);

			// Turning multiplier
			float  turnSpeed = controller->getTurningSpeed() * dt;
			// Thrust multiplier
			float  thrustPower = (controller->getThrustPower()+getSpeedBoost(ship, controller->getThrustPower())) * dt;

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

			m_thrustVec += thrustVec;
			m_angularVec += angularVec;


			if (input.stateSwitchInput != 0)
			{
				m_thrustVec = AglVector3::zero();
				m_angularVec = AglVector3::zero();
				ship->removeComponent(ComponentType::TAG_ShipFlyMode); // Disable this state...
				ship->addTag(ComponentType::TAG_ShipEditMode, new ShipEditMode_TAG()); // ...and switch to edit state.
				ship->applyComponentChanges();
			}

			if(static_cast<TimerSystem*>(m_world->getSystem(SystemType::TimerSystem))->
				checkTimeInterval(TimerIntervals::Every16Millisecond))
			{
				/************************************************************************/
				/* Send the thrust packet to the server!								*/
				/************************************************************************/

				NetworkSynced* netSync = static_cast<NetworkSynced*>(ship->getComponent(
					ComponentType::NetworkSynced));
				sendThrustPacketToServer(netSync,m_thrustVec, m_angularVec);

				m_thrustVec = AglVector3();
				m_angularVec = AglVector3();
			}


		}
	}
}

//Anton - 9/1-13
float ShipFlyControllerSystem::getSpeedBoost(Entity* p_entity, float p_baseThrust)
{
	ConnectionPointSet* cps = static_cast<ConnectionPointSet*>(
		p_entity->getComponent( ComponentType::ComponentTypeIdx::ConnectionPointSet ) );
	if (!cps)
		return 0;

	float speedBoost = 0;
	vector<ConnectionPoint> list = cps->m_connectionPoints;
	for (unsigned int i = 0; i < list.size(); i++)
	{
		if (list[i].cpConnectedEntity >= 0)
		{
			Entity* module = m_world->getEntity(list[i].cpConnectedEntity);

			SpeedBoosterModule* booster = static_cast<SpeedBoosterModule*>(
				module->getComponent( ComponentType::ComponentTypeIdx::SpeedBoosterModule ) );

			if (booster)
				speedBoost += 5.0f*p_baseThrust;
		}
	}
	return speedBoost;
}

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

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
				checkTimeInterval(TimerIntervals::Every16Millisecond))
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

void ShipFlyControllerSystem::handleTransformInterpolation( ShipFlyController* p_controller, 
														   Transform* p_transform )
{
	// fetch data
	// init
	float dt = m_world->getDelta();
	if (!p_controller->m_transformBuffer.empty() &&
		p_controller->m_currentTransformTimestamp<0.0f) // if no data gotten yet, init
	{
		ShipFlyController::TransformGoal front = p_controller->m_transformBuffer.front();
		p_controller->m_transformBuffer.pop();		
		p_controller->m_currentTransformTimestamp = front.timestamp;
		p_controller->m_startTransformTimestamp = front.timestamp;
		p_controller->m_goalTransformTimestamp = front.timestamp;

		p_controller->m_startTranslation = front.translation;
		p_controller->m_currentTranslation = front.translation;

		p_controller->m_startRotation = front.rotation;
		p_controller->m_currentRotation = front.rotation;

		p_controller->m_startScale = front.scale;
		p_controller->m_currentScale = front.scale;

	}
	if (!p_controller->m_transformBuffer.empty() &&
		p_controller->m_currentTransformTimestamp>0.0f) // get goal
	{
		// get new data if we have catched up
		// while timestamp batched at same time and there is still data
		while (p_controller->m_currentTransformTimestamp>p_controller->m_goalTransformTimestamp     
				&& !p_controller->m_transformBuffer.empty())					
		{
			ShipFlyController::TransformGoal front = p_controller->m_transformBuffer.front(); // fetch new front
			p_controller->m_startTranslation = p_controller->m_currentTranslation; // set our start value to current
			p_controller->m_startRotation = p_controller->m_currentRotation;
			p_controller->m_startScale = p_controller->m_currentScale;

			p_controller->m_goalTranslation = front.translation; // set our goal to the new front
			p_controller->m_goalRotation = front.rotation;
			p_controller->m_goalScale = front.scale;

			p_controller->m_startTransformTimestamp = p_controller->m_currentTransformTimestamp; // set set start time to current
			p_controller->m_goalTransformTimestamp = front.timestamp; // set our goal timestamp to the new front
			p_controller->m_transformBuffer.pop(); // pop old front
		}
	}

	DEBUGPRINT(( ("\nQueue size: "+toString(p_controller->m_transformBuffer.size())+"\n").c_str() ));



	// interpolate
	if (p_controller->m_currentTransformTimestamp<=p_controller->m_goalTransformTimestamp)
	{
		p_controller->m_currentTransformTimestamp += dt;
		float currentT = p_controller->m_currentTransformTimestamp;
		float startT = p_controller->m_startTransformTimestamp;
		float endT = p_controller->m_goalTransformTimestamp;
		float t = (currentT-startT) / max(1.0f,(endT-startT));
		DEBUGPRINT(( ("\nCurrent lerp: "+toString(t)+"\n").c_str() ));
		AglVector3 lerpedTranslation = AglVector3::lerp(p_controller->m_startTranslation,
			p_controller->m_goalTranslation,t);	
		AglQuaternion slerpedRotation = AglQuaternion::slerp(p_controller->m_startRotation,
			p_controller->m_goalRotation,t,true);	
		slerpedRotation.normalize();
		AglVector3 lerpedScale= AglVector3::lerp(p_controller->m_startScale,
			p_controller->m_goalScale,t);
		p_controller->m_currentTranslation = lerpedTranslation;
		p_controller->m_currentRotation = slerpedRotation;
		p_controller->m_currentScale = lerpedScale;
	}


	// update
	p_transform->setTranslation(p_controller->m_currentTranslation);
	p_transform->setRotation(p_controller->m_currentRotation);
	p_transform->setScale(p_controller->m_currentScale);

}


// void ShipFlyControllerSystem::sendTransformPacketToServer( NetworkSynced* p_syncedInfo, Transform* p_transform )
// {
// 	EntityUpdatePacket transformPacket;
// 	transformPacket.
// }

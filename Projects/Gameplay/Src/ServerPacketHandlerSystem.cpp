#include "ServerPacketHandlerSystem.h"
#include "ServerPickingSystem.h"
#include "ShipModulesControllerSystem.h"
#include "NetSyncedPlayerScoreTrackerSystem.h"
#include "ServerClientInfoSystem.h"

// Components
#include "Transform.h"
#include "NetworkSynced.h"
#include "PhysicsBody.h"
#include "ShipModule.h"
#include "SpeedBoosterModule.h"
#include "LookAtEntity.h"
#include "GameplayTags.h"

// NetComm
#include <TcpServer.h>

#include "PacketType.h"
#include "EntityType.h"
#include "PhysicsSystem.h"
#include "TimerSystem.h"
#include "ThrustPacket.h"
#include "PingPacket.h"
#include "PongPacket.h"
#include "RayPacket.h"
#include "UpdateClientStatsPacket.h"
#include "HighlightSlotPacket.h"
#include "SimpleEventPacket.h"
#include "PlayerScore.h"
#include "CameraControlPacket.h"
#include "ShipConnectionPointHighlights.h"




ServerPacketHandlerSystem::ServerPacketHandlerSystem( TcpServer* p_server )
	: EntitySystem( SystemType::ServerPacketHandlerSystem, 3,
	ComponentType::NetworkSynced, ComponentType::ShipFlyController,
	ComponentType::PhysicsBody )
{
	m_server = p_server;
}

ServerPacketHandlerSystem::~ServerPacketHandlerSystem()
{

}


void ServerPacketHandlerSystem::initialize()
{
	m_physics = static_cast<PhysicsSystem*>(
		m_world->getSystem( SystemType::PhysicsSystem ) );
}

void ServerPacketHandlerSystem::processEntities( const vector<Entity*>& p_entities )
{
	float dt = m_world->getDelta();

	while( m_server->hasNewPackets() )
	{
		Packet packet = m_server->popNewPacket();

		char packetType;
		packetType = packet.getPacketType();
		
		if(packetType == (char)PacketType::ThrustPacket)
		{
			// =========================================
			// THRUSTPACKET
			// =========================================
			ThrustPacket thrustPacket;
			thrustPacket.unpack( packet );

			Entity* ship = m_world->getEntity(thrustPacket.entityId);

			PhysicsBody* physicsBody = static_cast<PhysicsBody*>
				(ship->getComponent(ComponentType::PhysicsBody));


			ConnectionPointSet* connected = static_cast<ConnectionPointSet*>(
				ship->getComponent(ComponentType::ConnectionPointSet) );

			ShipConnectionPointHighlights* highlights = static_cast<ShipConnectionPointHighlights*>(
				ship->getComponent(ComponentType::ShipConnectionPointHighlights) );
			
			AglVector3 boostVector = AglVector3(0, 0, 0);

			for (unsigned int i=0;i<ShipConnectionPointHighlights::slots;i++)
			{
				if (highlights->slotStatus[i])
				{
					if (connected->m_connectionPoints[i].cpConnectedEntity >= 0)
					{
						Entity* shipModule = m_world->getEntity(connected->m_connectionPoints[i].cpConnectedEntity);
						ShipModule* module = static_cast<ShipModule*>(shipModule->getComponent(ComponentType::ShipModule));
						SpeedBoosterModule* boostmodule = static_cast<SpeedBoosterModule*>(shipModule->getComponent(ComponentType::SpeedBoosterModule));
						if (module->getActive() && boostmodule)
							boostVector = thrustPacket.thrustVector*3;
					}
				}
			}

			m_physics->applyImpulse( physicsBody->m_id, (thrustPacket.thrustVector+boostVector),
				thrustPacket.angularVector );
			
		}
		else if ( packetType == (char)PacketType::CameraControlPacket)
		{
			// =========================================
			// CAMERACONTROLPACKET
			// =========================================
			// Camera controlling by client
			CameraControlPacket cameraControlPacket;
			cameraControlPacket.unpack( packet );

			Entity* entity = m_world->getEntity(cameraControlPacket.entityId);

			if (entity)
			{
				// Handle lookat tags based on state
				// Maybe the tags are to cumbersome? 
				// Implement a state component for camera instead?
				LookAtEntity* lookAt = static_cast<LookAtEntity*>
					(entity->getComponent(ComponentType::LookAtEntity)); 
				if (lookAt)
				{
					lookAt->setOrbitMovement(cameraControlPacket.movement);
					// get lookAt tags if they exist
					LookAtFollowMode_TAG* lookAtFollow=NULL;
					LookAtOrbitMode_TAG* lookAtOrbit=NULL;
					Transform* cameraTransform=NULL;

					Component* t = entity->getComponent(ComponentType::ComponentTypeIdx::TAG_LookAtFollowMode);
					if (t!=NULL) lookAtFollow = static_cast<LookAtFollowMode_TAG*>(t);

					t = entity->getComponent( ComponentType::ComponentTypeIdx::TAG_LookAtOrbitMode );
					if (t!=NULL) lookAtOrbit = static_cast<LookAtOrbitMode_TAG*>(t);

					t = entity->getComponent( ComponentType::ComponentTypeIdx::Transform );
					if (t!=NULL) cameraTransform = static_cast<Transform*>(t);


					// handle "state" switch
					if (lookAtFollow && !lookAtOrbit &&
						cameraControlPacket.state==PlayerStates::editState)
					{
						lookAt->setOrbitOffset(AglQuaternion::identity());
						entity->removeComponent(ComponentType::TAG_LookAtFollowMode); // Disable this state...
						entity->addComponent(ComponentType::TAG_LookAtOrbitMode, new LookAtOrbitMode_TAG());  // ...and switch to orbit state.
						entity->applyComponentChanges();
					}
					else if (lookAtOrbit && !lookAtFollow && 
						cameraControlPacket.state==PlayerStates::steeringState)				
					{
						{
							entity->removeComponent(ComponentType::TAG_LookAtOrbitMode); // Disable this state...
							entity->addComponent(ComponentType::TAG_LookAtFollowMode, new LookAtFollowMode_TAG());  // ...and switch to follow state.
							entity->applyComponentChanges();
						}
					}
				}

			}

		}
		else if( packetType == (char)PacketType::Ping )
		{
			// =========================================
			// PINGPACKET
			// =========================================
			PingPacket pingPacket;
			pingPacket.unpack( packet );

			Packet response((char)PacketType::Pong);
			response << pingPacket.timeStamp;

			m_server->unicastPacket( response, packet.getSenderId() );
		}
		else if( packetType == (char)PacketType::Pong)
		{
			// =========================================
			// PONGPACKET
			// =========================================
			//auto clientInfo = static_cast<ClientInfo*>(m_world->getEntityManager()->get)
			

			float totalElapsedTime = m_world->getElapsedTime();
			float timeWhenSent;

			PongPacket pongPacket;
			pongPacket.unpack( packet );
			timeWhenSent = pongPacket.timeStamp;

			/************************************************************************/
			/* Convert from seconds to milliseconds.								*/
			/************************************************************************/
			float ping = (totalElapsedTime - timeWhenSent)*1000.0f;
			//m_clients[packet.getSenderId()] = info;

			auto clientInfoSys = static_cast<ServerClientInfoSystem*>(
				m_world->getSystem(SystemType::ServerClientInfoSystem));

			vector<Entity*> clientInfoEntities = clientInfoSys->getActiveEntities();
			for (unsigned int i = 0; i < clientInfoEntities.size(); i++)
			{
				auto clientInfo = static_cast<ClientInfo*>(
					clientInfoEntities[i]->getComponent(ComponentType::ClientInfo));

				if (clientInfo->id == packet.getSenderId())
				{
					clientInfo->ping = ping;
				}
			}
		}	
		else if (packetType == (char)PacketType::RayPacket)
		{
			// =========================================
			// RAYPACKET
			// =========================================
			ServerPickingSystem* picking = 
				static_cast<ServerPickingSystem*>(m_world->getSystem(SystemType::ServerPickingSystem));

			RayPacket rayPacket;
			rayPacket.unpack( packet );
			picking->setRay(packet.getSenderId(), rayPacket.o, rayPacket.d);
		}
		else if (packetType == (char)PacketType::ModuleHighlightPacket)
		{
			// =========================================
			// MODULEHIGHLIGHTPACKET
			// =========================================
			ShipModulesControllerSystem* modsystem = 
				static_cast<ShipModulesControllerSystem*>(m_world->getSystem(SystemType::ShipModulesControllerSystem));

			HighlightSlotPacket hp;
			hp.unpack( packet );
			modsystem->addHighlightEvent(hp.id, packet.getSenderId());
		}
		else if (packetType == (char)PacketType::SimpleEvent)
		{
			// =========================================
			// SIMPLEEVENTPACKET
			// =========================================
			ShipModulesControllerSystem* modsystem = 
				static_cast<ShipModulesControllerSystem*>(m_world->getSystem(SystemType::ShipModulesControllerSystem));

			ServerPickingSystem* pickSystem = 
				static_cast<ServerPickingSystem*>(m_world->getSystem(SystemType::ServerPickingSystem));

			SimpleEventPacket sep;
			sep.unpack( packet );
			if (sep.type == SimpleEventType::ACTIVATE_MODULE)
				modsystem->addActivateEvent(packet.getSenderId());
			else if (sep.type == SimpleEventType::DEACTIVATE_MODULE)
				modsystem->addDeactivateEvent(packet.getSenderId());
			else if (sep.type == SimpleEventType::ROTATE_ADD)
				pickSystem->addRotationEvent(1, packet.getSenderId());
			else if (sep.type == SimpleEventType::ROTATE_SUB)
				pickSystem->addRotationEvent(-1, packet.getSenderId());
			else if (sep.type == SimpleEventType::ROTATE_NONE)
				pickSystem->addRotationEvent(0, packet.getSenderId());
			else if (sep.type == SimpleEventType::ROTATE_90_ADD)
				pickSystem->add90RotationEvent(1, packet.getSenderId());
			else if (sep.type == SimpleEventType::ROTATE_90_SUB)
				pickSystem->add90RotationEvent(-1, packet.getSenderId());
			else if (sep.type == SimpleEventType::ACTIVATE_PICK)
				pickSystem->setEnabled(packet.getSenderId(), true);
			else if (sep.type == SimpleEventType::DEACTIVATE_PICK)
				pickSystem->setEnabled(packet.getSenderId(), false);
			else if (sep.type == SimpleEventType::RELEASE_PICK)
				pickSystem->setReleased(packet.getSenderId());
		}
	}
	
	if( static_cast<TimerSystem*>(m_world->getSystem(SystemType::TimerSystem))->
		checkTimeInterval(TimerIntervals::HalfSecond))
	{
		float timeStamp = m_world->getElapsedTime();

		PingPacket pingPacket;
		pingPacket.timeStamp = timeStamp;

		m_server->broadcastPacket( pingPacket.pack() );
	}
}

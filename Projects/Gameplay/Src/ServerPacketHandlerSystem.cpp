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
#include "ShipManagerSystem.h"
#include "SlotParticleEffectPacket.h"
#include "EditSphereUpdatePacket.h"
#include "ServerGameState.h"
#include "ServerStateSystem.h"
#include "EntityCreationPacket.h"
#include "ServerStaticObjectsSystem.h"
#include "StaticProp.h"
#include <vector>
#include "EntityFactory.h"
#include "ServerDynamicObjectsSystem.h"
#include "ChangeStatePacket.h"
#include <DebugUtil.h>
#include "ServerWelcomeSystem.h"
#include "PlayerInfo.h"



ServerPacketHandlerSystem::ServerPacketHandlerSystem( TcpServer* p_server )
	: EntitySystem( SystemType::ServerPacketHandlerSystem, 3,
	ComponentType::NetworkSynced, ComponentType::ShipFlyController,
	ComponentType::PhysicsBody )
{
	m_server = p_server;
	m_stateSystem = NULL;
	m_finishedLoadingPlayers = 0;
	m_readyLoadingPlayers = 0;
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

	m_stateSystem = static_cast<ServerStateSystem*>(
		m_world->getSystem(SystemType::ServerStateSystem));

	switch (m_stateSystem->getCurrentState())
	{
	case ServerStates::INGAME:
		{
			handleIngame();
			break;
		}
	case ServerStates::LOADING:
		{
			handleLoading();
			break;
		}
	case ServerStates::LOBBY:
		{
			handleLobby();
			break;
		}
	case ServerStates::SENTALLPACKETS:
		{
			handleSentAllPackets();
			break;
		}
	default:
		break;
	}
	/*
	if( static_cast<TimerSystem*>(m_world->getSystem(SystemType::TimerSystem))->
		checkTimeInterval(TimerIntervals::HalfSecond))
	{
		float timeStamp = m_world->getElapsedTime();

		PingPacket pingPacket;
		pingPacket.timeStamp = timeStamp;

		m_server->broadcastPacket( pingPacket.pack() );
	}
	*/
}
void ServerPacketHandlerSystem::createAndBroadCastShip( int p_clientIdentity, int p_playerID)
{

	Entity* newShip = createTheShipEntity(p_clientIdentity, p_playerID);
	m_world->addEntity(newShip);
	Transform* transformComp = static_cast<Transform*>(newShip->getComponent(
		ComponentType::Transform));

	// also create a camera
	Entity* playerCam = m_world->createEntity();
	Component* component = new LookAtEntity(newShip->getIndex(),
		AglVector3(0,7,-38),
		13.0f,
		10.0f,
		3.0f,
		40.0f);
	playerCam->addComponent( ComponentType::LookAtEntity, component );
	playerCam->addComponent( ComponentType::Transform, new Transform( 
		transformComp->getMatrix() ) );
	// default tag is follow
	playerCam->addTag(ComponentType::TAG_LookAtFollowMode, new LookAtFollowMode_TAG() );
	playerCam->addComponent( ComponentType::NetworkSynced, 
		new NetworkSynced( playerCam->getIndex(), p_clientIdentity, EntityType::PlayerCamera ));
	m_world->addEntity(playerCam);

	/************************************************************************/
	/* Send the information about the new clients ship to all other players */
	/************************************************************************/
	EntityCreationPacket data;
	data.entityType		= static_cast<char>(EntityType::Ship);
	data.owner			= p_clientIdentity;
	data.playerID		= p_playerID;
	data.networkIdentity= newShip->getIndex();
	data.translation	= transformComp->getTranslation();
	data.rotation		= transformComp->getRotation();
	data.scale			= transformComp->getScale();
	data.miscData		= playerCam->getIndex();

	m_server->broadcastPacket(data.pack());
}

Entity* ServerPacketHandlerSystem::createTheShipEntity(int p_newlyConnectedClientId, int p_playerID)
{
	/************************************************************************/
	/* Creating the ship entity.											*/
	/************************************************************************/
	EntityFactory* factory = static_cast<EntityFactory*>(m_world->getSystem(SystemType::EntityFactory));

	Entity* e = factory->entityFromRecipeOrFile( "ServerShip", "Assemblages/ServerShip.asd");

	e->addComponent(ComponentType::ShipConnectionPointHighlights, 
		new ShipConnectionPointHighlights());

	e->addComponent( ComponentType::NetworkSynced, 
		new NetworkSynced( e->getIndex(), p_newlyConnectedClientId, p_playerID, EntityType::Ship ));

	e->addComponent(ComponentType::TAG_Ship, new Ship_TAG());

	e->addComponent(ComponentType::PlayerScore, new PlayerScore());

	return e;
}

void ServerPacketHandlerSystem::printPacketTypeNotHandled( string p_state, int p_packetType )
{
	DEBUGPRINT((("SERVER: Not handled("+p_state+"): " +
		toString(p_packetType) + "\n").c_str()));
}

void ServerPacketHandlerSystem::handleIngame()
{
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

						//Send a packet back to the client telling him where connection points are
						ShipModulesControllerSystem* smcs = static_cast<ShipModulesControllerSystem*>(m_world->getSystem(SystemType::ShipModulesControllerSystem));
						smcs->setEditMode(true);

						ShipManagerSystem* sms = static_cast<ShipManagerSystem*>(m_world->getSystem(SystemType::ShipManagerSystem));
						vector<FreeSlotData> slots = sms->findFreeConnectionPoints(packet.getSenderId());

						for (unsigned int i = 0; i < slots.size(); i++)
						{
							NetworkSynced* netSync = static_cast<NetworkSynced*>(slots[i].parent->getComponent(ComponentType::NetworkSynced));

							SlotParticleEffectPacket slotPacket;
							slotPacket.translationOffset = slots[i].offset.GetTranslation();
							slotPacket.forwardDirection = slots[i].offset.GetForward();
							slotPacket.slot = slots[i].index;
							slotPacket.networkIdentity = netSync->getNetworkIdentity();
							slotPacket.active = true;

							m_server->unicastPacket(slotPacket.pack(), packet.getSenderId() );
						}

						//Send a packet back to the client telling him how the edit sphere should be oriented
						EditSphereUpdatePacket editSphereUpdate;
						AglBoundingSphere bs = sms->findEditSphere(packet.getSenderId());
						editSphereUpdate.m_offset = bs.position;
						editSphereUpdate.m_radius = bs.radius;

						m_server->unicastPacket(editSphereUpdate.pack(), packet.getSenderId());
					}
					else if (lookAtOrbit && !lookAtFollow && 
						cameraControlPacket.state==PlayerStates::steeringState)				
					{
						{
							entity->removeComponent(ComponentType::TAG_LookAtOrbitMode); // Disable this state...
							entity->addComponent(ComponentType::TAG_LookAtFollowMode, new LookAtFollowMode_TAG());  // ...and switch to follow state.
							entity->applyComponentChanges();

							//Send a packet back to the client telling him no connection points should be drawn

							ShipModulesControllerSystem* smcs = static_cast<ShipModulesControllerSystem*>(m_world->getSystem(SystemType::ShipModulesControllerSystem));
							smcs->setEditMode(false);

							SlotParticleEffectPacket slotPacket;
							slotPacket.translationOffset = AglVector3::zero();
							slotPacket.slot = -1;
							slotPacket.networkIdentity = -1;
							slotPacket.active = false;

							m_server->unicastPacket(slotPacket.pack(), packet.getSenderId() );
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
}

void ServerPacketHandlerSystem::handleLobby()
{
	while( m_server->hasNewPackets() )
	{
		Packet packet = m_server->popNewPacket();

		char packetType;
		packetType = packet.getPacketType();

		if(packetType == (char)PacketType::ChangeStatePacket){
			ChangeStatePacket statePacket;
			statePacket.unpack(packet);

			if(statePacket.m_gameState == GameStates::INITGAME){

				ChangeStatePacket newState;
				newState.m_serverState = ServerStates::LOADING;
				m_stateSystem->setQueuedState(ServerStates::LOADING);
				m_server->broadcastPacket(newState.pack());
			}
		}
		else if(packetType == (char)PacketType::PlayerInfo){
			PlayerInfo playerInfo;
			playerInfo.unpack(packet);
			ServerWelcomeSystem* welcomeSystem  = static_cast<ServerWelcomeSystem*>(
				m_world->getSystem(SystemType::ServerWelcomeSystem));
			
			welcomeSystem->addPlayer(playerInfo.playerID, playerInfo.playerName);
			welcomeSystem->sendBrodcastAllPlayers();
		}
		else
		{
			printPacketTypeNotHandled("Lobby", (int)packetType);
		}
	}
}

void ServerPacketHandlerSystem::handleLoading()
{
	while( m_server->hasNewPackets() )
	{
		Packet packet = m_server->popNewPacket();

		char packetType;
		packetType = packet.getPacketType();

		if(packetType == (char)PacketType::ChangeStatePacket){
			ChangeStatePacket statePacket;
			statePacket.unpack(packet);

			if(statePacket.m_gameState == GameStates::LOADING){
				m_readyLoadingPlayers++;
			}
		}
	}

	ServerWelcomeSystem* welcomeSystem  = static_cast<ServerWelcomeSystem*>(
		m_world->getSystem(SystemType::ServerWelcomeSystem));

	if(m_readyLoadingPlayers == welcomeSystem->getTotalOfConnectedPlayers()){
		/************************************************************************/
		/* Send the already networkSynced objects located on the server to the	*/
		/* newly connected client.												*/
		/************************************************************************/ 
		NetworkSynced* netSync;
		Transform* transform;
		vector<Entity*> dynamicEntities = static_cast<ServerDynamicObjectsSystem*>(
			m_world->getSystem(SystemType::ServerDynamicObjectsSystem))->getActiveEntities();
		for( unsigned int i=0; i<dynamicEntities.size(); i++ )
		{
			int entityId = dynamicEntities[i]->getIndex();
			netSync = (NetworkSynced*)m_world->getComponentManager()->
				getComponent( entityId, ComponentType::NetworkSynced );

			transform = (Transform*)m_world->getComponentManager()->
				getComponent( entityId, ComponentType::Transform );

			EntityCreationPacket data;
			data.entityType		= static_cast<char>(netSync->getNetworkType());
			data.owner			= netSync->getNetworkOwner();
			data.playerID		= netSync->getPlayerID();
			data.networkIdentity = netSync->getNetworkIdentity();

			data.meshInfo		 = 0; //Temp
			if (transform)
			{
				data.translation	= transform->getTranslation();
				data.rotation		= transform->getRotation();
				data.scale			= transform->getScale();
			}

			///MESH INFO MUST BE MADE INTO A COMPONENT
			//data.meshInfo		= 1;

			m_server->broadcastPacket( data.pack() );
		}

		/************************************************************************/
		/* Send static objects to the newly connected client.					*/
		/************************************************************************/
		vector<Entity*> entities = static_cast<ServerStaticObjectsSystem*>(m_world->
			getSystem(SystemType::ServerStaticObjectsSystem))->getStaticObjects();

		for (unsigned int i= 0; i < entities.size(); i++)
		{
			transform = static_cast<Transform*>(entities[i]->
				getComponent(ComponentType::Transform));

			StaticProp* prop = static_cast<StaticProp*>(entities[i]->
				getComponent(ComponentType::StaticProp));

			EntityCreationPacket data;
			data.entityType = static_cast<char>(EntityType::Other);
			data.owner = -1; //NO OWNER
			data.networkIdentity = entities[i]->getIndex();
			data.translation = transform->getTranslation();
			data.rotation = transform->getRotation();
			data.scale = transform->getScale();
			data.isLevelProp = prop->isLevelPiece;
			data.meshInfo = prop->meshInfo;  

			//				packets.push( packet );
			m_server->broadcastPacket( data.pack() );
		}

		/************************************************************************/
		/* Create and send the ship entities.									*/
		/************************************************************************/
		for(unsigned int i=0; i<m_server->getActiveConnections().size(); i++)
		{
			createAndBroadCastShip(m_server->getActiveConnections().at(i), (int)i);
		}

		ChangeStatePacket changeState;
		changeState.m_serverState = ServerStates::SENTALLPACKETS;
		m_server->broadcastPacket(changeState.pack());
		m_stateSystem->setQueuedState(ServerStates::SENTALLPACKETS);
	}
}

void ServerPacketHandlerSystem::handleSentAllPackets()
{
	while( m_server->hasNewPackets() )
	{
		Packet packet = m_server->popNewPacket();

		char packetType;
		packetType = packet.getPacketType();

		if(packetType == (char)PacketType::ChangeStatePacket){
			ChangeStatePacket statePacket;
			statePacket.unpack(packet);

			if(statePacket.m_gameState == GameStates::FINISHEDLOADING){

				m_finishedLoadingPlayers++;

				ServerWelcomeSystem* welcomeSystem  = static_cast<ServerWelcomeSystem*>(
					m_world->getSystem(SystemType::ServerWelcomeSystem));

				if(m_finishedLoadingPlayers == welcomeSystem->getTotalOfConnectedPlayers()){
					m_stateSystem->setQueuedState(ServerStates::INGAME);
					statePacket.m_gameState = GameStates::NONE;
					statePacket.m_serverState = ServerStates::INGAME;
					m_server->broadcastPacket(statePacket.pack());
				}
			}
		}
		else{
			printPacketTypeNotHandled("Sent All Packet", (int)packetType);
		}
	}
}

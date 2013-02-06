#include "ServerWelcomeSystem.h"
#include <iostream>
#include <queue>
#include <vector>

#include <SystemType.h>
#include <ProcessMessageTerminate.h>
#include <ThreadSafeMessaging.h>
#include <TcpListenerProcess.h>
#include <DebugUtil.h>
#include <ToString.h>
#include <boost/thread/thread.hpp>

#include "PacketType.h"
#include "EntityType.h"

#include "ServerStaticObjectsSystem.h"
#include "ServerClientInfoSystem.h"

// Components:
#include "Transform.h"
#include "NetworkSynced.h"
#include "ShipFlyController.h"
#include "BodyInitData.h"
#include "PhysicsBody.h"
#include "PlayerScore.h"
#include "ConnectionPointSet.h"
#include "GameplayTags.h"
#include "StaticProp.h"
#include "ClientInfo.h"
#include "LookAtEntity.h"

// Packets
#include "EntityCreationPacket.h"
#include "WelcomePacket.h"

#include <Globals.h>


ServerWelcomeSystem::ServerWelcomeSystem( TcpServer* p_server, 
										 int p_activePort/* =1337 */ )
	: EntitySystem( SystemType::NetworkListenerSystem, 1, ComponentType::NetworkSynced)
{
	m_server = p_server;
	m_activePort = p_activePort;
}

ServerWelcomeSystem::~ServerWelcomeSystem()
{
	m_server->stopListening();
}

void ServerWelcomeSystem::processEntities( const vector<Entity*>& p_entities )
{
	/************************************************************************/
	/* NOT USED RIGHT NOW! CLIENTS CANT DISCONNECT.							*/
	/************************************************************************/
	while (m_server->hasNewDisconnections())
	{
		int id = m_server->popNewDisconnection();

		// When a client is disconnecting, then all other clients must know this.
		// At this point, the disconnecting client is not in the active connections list.
//		Packet dcPacket;
//		dcPacket 
//			<< (char)PacketType::ClientDisconnect
//			<< (char)id; 
//			
//		m_server->broadcastPacket( Packet() );

		for (unsigned int index = 0; index < p_entities.size(); index++)
		{
			NetworkSynced* netSync = static_cast<NetworkSynced*>(
				m_world->getComponentManager()->getComponent( p_entities[index],
					ComponentType::NetworkSynced ) );

			// HACK: This deletion is what caused the magical crashes all the time.
			// This should be solved as soon as possible.
			//if (netSync->getNetworkIdentity() == id)
			//	m_world->deleteEntity(p_entities[index]);
		}

		// If a client has disconnected, then the clientinfo should be removed?
		auto clientInfoSys = static_cast<ServerClientInfoSystem*>(
			m_world->getSystem(SystemType::ServerClientInfoSystem));
		vector<Entity*> clientInfoEntities = clientInfoSys->getActiveEntities();
		for (unsigned int i = 0; i < clientInfoEntities.size(); i++)
		{
			auto clientInfo = static_cast<ClientInfo*>(
				clientInfoEntities[i]->getComponent(ComponentType::ClientInfo));

			if (clientInfo->id == id)
				m_world->deleteEntity(clientInfoEntities[i]);
		}
	}

	/************************************************************************/
	/* It goes here if there are new clients that has connected to the		*/
	/* server.																*/
	/************************************************************************/
	if ( m_server->isListening() )
	{
		while( m_server->hasNewConnections() )
		{
			int id = m_server->popNewConnection();

			// Creates a new client info entity for the newly connected client
			createClientInfoEntity(id);

			/************************************************************************/
			/* Send the newly connected client a welcome packet with all inclusive. */
			/************************************************************************/
			sendWelcomePacket(id);

			/************************************************************************/
			/* Send the already networkSynced objects located on the server to the	*/
			/* newly connected client.												*/
			/************************************************************************/ 
			NetworkSynced* netSync;
			Transform* transform;
			for( unsigned int i=0; i<p_entities.size(); i++ )
			{
				int entityId = p_entities[i]->getIndex();
				netSync = (NetworkSynced*)m_world->getComponentManager()->
					getComponent( entityId, ComponentType::NetworkSynced );

				transform = (Transform*)m_world->getComponentManager()->
					getComponent( entityId, ComponentType::Transform );

				EntityCreationPacket data;
				data.entityType		= static_cast<char>(netSync->getNetworkType());
				data.owner			= netSync->getNetworkOwner();
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

				m_server->unicastPacket( data.pack(), id );
			}

			/************************************************************************/
			/* Send static objects to the newly connected client.					*/
			/************************************************************************/
			vector<Entity*> entities = static_cast<ServerStaticObjectsSystem*>(m_world->
				getSystem(SystemType::ServerStaticObjectsSystem))->getStaticObjects();

//			queue< Packet > packets;

			for (unsigned int i= 0; i < entities.size(); i++)
			{
//				if( i%20 == 0 )
//				{
//					boost::this_thread::sleep(boost::posix_time::milliseconds(1));
//				}
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
				m_server->unicastPacket( data.pack(), id );
			}
//			m_server->unicastPacketQueue( packets, id );
			
		}
	}
}

void ServerWelcomeSystem::initialize()
{
	m_server->startListening(m_activePort);
}

void ServerWelcomeSystem::sendWelcomePacket(int p_newlyConnectedClientId)
{
	// Give the new client its Network Identity.
	WelcomePacket welcomePacket;
	welcomePacket.clientNetworkIdentity = p_newlyConnectedClientId;
	m_server->unicastPacket( welcomePacket.pack(), p_newlyConnectedClientId );

	Transform* transformComp = new Transform( 
		0, 0, -10*static_cast<float>(p_newlyConnectedClientId));
	Entity* newShip = createTheShipEntity(p_newlyConnectedClientId, transformComp);
	m_world->addEntity(newShip);

	// also create a camera
	Entity* playerCam = m_world->createEntity();
	Component* component = new LookAtEntity(newShip->getIndex(),
		AglVector3(0,7,-28),
		13.0f,
		10.0f,
		3.0f,
		40.0f);
	playerCam->addComponent( ComponentType::LookAtEntity, component );
	playerCam->addComponent( ComponentType::Transform, new Transform( transformComp->getMatrix() ) );
	// default tag is follow
	playerCam->addTag(ComponentType::TAG_LookAtFollowMode, new LookAtFollowMode_TAG() );
	playerCam->addComponent( ComponentType::NetworkSynced, 
		new NetworkSynced( playerCam->getIndex(), p_newlyConnectedClientId, EntityType::PlayerCamera ));
	m_world->addEntity(playerCam);

	/************************************************************************/
	/* Send the information about the new clients ship to all other players */
	/************************************************************************/
	EntityCreationPacket data;
	data.entityType		= static_cast<char>(EntityType::Ship);
	data.owner			= p_newlyConnectedClientId;
	data.networkIdentity = newShip->getIndex();
	data.translation	= transformComp->getTranslation();
	data.rotation		= transformComp->getRotation();
	data.scale			= transformComp->getScale();
	data.miscData		= playerCam->getIndex();

	m_server->broadcastPacket(data.pack());
}

Entity* ServerWelcomeSystem::createTheShipEntity(int p_newlyConnectedClientId, 
												 Transform* p_shipTransform)
{
	/************************************************************************/
	/* Creating the ship entity.											*/
	/************************************************************************/
	Entity* e = m_world->createEntity();

	e->addComponent(ComponentType::Transform, p_shipTransform);
	e->addComponent( ComponentType::NetworkSynced, 
		new NetworkSynced( e->getIndex(), p_newlyConnectedClientId, EntityType::Ship ));
	
	e->addComponent( ComponentType::PhysicsBody, 
		new PhysicsBody() );

	e->addComponent( ComponentType::BodyInitData, 
		new BodyInitData( p_shipTransform->getTranslation(),
		AglQuaternion::identity(),
		AglVector3(1, 1, 1), AglVector3(0, 0, 0), 
		AglVector3(0, 0, 0), 0, 
		BodyInitData::DYNAMIC, 
		BodyInitData::COMPOUND));

	ConnectionPointSet* connectionPointSet = new ConnectionPointSet();
	connectionPointSet->m_connectionPoints.push_back(ConnectionPoint(AglMatrix::createTranslationMatrix(AglVector3(4.5f, 0, 0))));
	connectionPointSet->m_connectionPoints.push_back(ConnectionPoint(AglMatrix::createTranslationMatrix(AglVector3(-4.5f, 0, 0))));
	connectionPointSet->m_connectionPoints.push_back(ConnectionPoint(AglMatrix::createTranslationMatrix(AglVector3(0, 4.5f, 0))));

	e->addComponent(ComponentType::ConnectionPointSet, connectionPointSet);

	e->addComponent(ComponentType::TAG_Ship, new Ship_TAG());

	e->addComponent(ComponentType::PlayerScore, new PlayerScore());

	return e;
}

void ServerWelcomeSystem::createClientInfoEntity( int p_newlyConnectedClientId )
{
	Entity* e = m_world->createEntity();
	e->addComponent(ComponentType::ClientInfo, new ClientInfo(p_newlyConnectedClientId));
	m_world->addEntity(e);
}

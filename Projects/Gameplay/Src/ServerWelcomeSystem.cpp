#include "ServerWelcomeSystem.h"
#include <iostream>
#include <queue>
#include <vector>

#include <SystemType.h>
#include <ProcessMessageTerminate.h>
#include <ThreadSafeMessaging.h>
#include <TcpListenerProcess.h>
#include <TcpServer.h>
#include <DebugUtil.h>
#include <ToString.h>
#include <boost/thread/thread.hpp>

#include "PacketType.h"
#include "EntityType.h"

#include "ServerStaticObjectsSystem.h"

// Components:
#include "Transform.h"
#include "NetworkSynced.h"
#include "ShipFlyController.h"
#include "BodyInitData.h"
#include "PhysicsBody.h"
#include "PlayerScore.h"

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

				Packet packet((char)PacketType::EntityCreation);
				packet << (char)netSync->getNetworkType() 
					<< netSync->getNetworkOwner() 
					<< entityId 
					<< transform->getTranslation() 
					<< transform->getRotation() 
					<< transform->getScale();

				m_server->unicastPacket( packet, id );
			}

			/************************************************************************/
			/* Send static objects to the newly connected client.					*/
			/************************************************************************/
			vector<Entity*> entities = static_cast<ServerStaticObjectsSystem*>(m_world->
				getSystem(SystemType::ServerStaticObjectsSystem))->getStaticObjects();

//			queue< Packet > packets;

			for (unsigned int i= 0; i < entities.size(); i++)
			{
				if( i%20 == 0 )
				{
					boost::this_thread::sleep(boost::posix_time::milliseconds(1));
				}
				transform = static_cast<Transform*>(entities[i]->
					getComponent(ComponentType::Transform));

				Packet packet((char)PacketType::EntityCreation);
				packet << (char)EntityType::StaticProp
					<< -1 //NO OWNER
					<< entities[i]->getIndex()
					<< transform->getTranslation()
					<< transform->getRotation()
					<< transform->getScale();
				
//				packets.push( packet );
				m_server->unicastPacket( packet, id );
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
	Packet identityPacket( (char)PacketType::WelcomePacket );
	identityPacket << p_newlyConnectedClientId;
	m_server->unicastPacket( identityPacket, p_newlyConnectedClientId );

	Transform* transformComp = new Transform( 
		0, 0, 10*static_cast<float>(p_newlyConnectedClientId));
	Entity* newShip = createTheShipEntity(p_newlyConnectedClientId, transformComp);
	m_world->addEntity(newShip);

	/************************************************************************/
	/* Send the information about the new clients ship to all other players */
	/************************************************************************/
	announceConnectedClients(newShip,p_newlyConnectedClientId, transformComp);
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

	return e;
}

void ServerWelcomeSystem::announceConnectedClients(Entity* p_entity, 
												  int p_newlyConnectedClientId, 
												  Transform* p_shipTransform)
{
	Packet newClientConnected(PacketType::EntityCreation);
	newClientConnected 
		<< (char)EntityType::Ship 
		<< p_newlyConnectedClientId 
		<< p_entity->getIndex() 
		<< p_shipTransform->getTranslation()
		<< p_shipTransform->getRotation()
		<< p_shipTransform->getScale();

	m_server->broadcastPacket(newClientConnected);
}

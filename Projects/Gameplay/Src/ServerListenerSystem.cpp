#include "ServerListenerSystem.h"
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

#include "PacketType.h"
#include "EntityType.h"

// Components:
#include "Transform.h"
#include "NetworkSynced.h"
#include "ShipController.h"
#include "BodyInitData.h"
#include "PhysicsBody.h"
#include "PlayerScore.h"

ServerListenerSystem::ServerListenerSystem( TcpServer* p_server )
	: EntitySystem( SystemType::NetworkListenerSystem, 1, ComponentType::NetworkSynced)
{
	m_server = p_server;
}

ServerListenerSystem::~ServerListenerSystem()
{
	m_server->stopListening();
}

void ServerListenerSystem::processEntities( const vector<Entity*>& p_entities )
{
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

	if ( m_server->isListening() )
	{
		while( m_server->hasNewConnections() )
		{
			int id = m_server->popNewConnection();

			// Give the new client its Network Identity.
			Packet identityPacket( (char)PacketType::InitCredentials );
			identityPacket << (char)EntityType::Identity << id;
			m_server->unicastPacket( identityPacket, id );

			// Create a new entity for the connecting client, and belonging components
			Entity* e = m_world->createEntity();

			Transform* transform = new Transform( (float)(id) * 10.0f, 0, 0 );
			NetworkSynced* netSync = 
				new NetworkSynced( e->getIndex(), id, EntityType::Ship );
			
			/************************************************************************/
			/* Creating the ship entity.											*/
			/************************************************************************/
			e->addComponent( ComponentType::Transform, transform);
			e->addComponent( ComponentType::NetworkSynced, netSync);
			e->addComponent( ComponentType::ShipController,
				new ShipController(5.0f, 50.0f));
			e->addComponent( ComponentType::PlayerScore,
				new PlayerScore(0) );

			e->addComponent( ComponentType::PhysicsBody, 
				new PhysicsBody() );

			e->addComponent( ComponentType::BodyInitData, 
				new BodyInitData( transform->getTranslation(),
								AglQuaternion::identity(),
								AglVector3(1, 1, 1), AglVector3(0, 0, 0), 
								AglVector3(0, 0, 0), 0, 
								BodyInitData::DYNAMIC, 
								BodyInitData::COMPOUND));
			m_world->addEntity( e );

			// When a client is connecting, the server must broadcast to all other
			// clients that a new client exists.
			// Packet needed: ON_CLIENT_CONNECT 
			//	data: clientId
			//	And entity creation.

			// Broadcast the new client's entity to all clients, even the new one.
			Packet newClientConnected(PacketType::EntityCreation);
			newClientConnected << (char)EntityType::Ship << id << e->getIndex() 
				<< transform->getTranslation() << transform->getRotation() 
				<< transform->getScale();
			
			m_server->broadcastPacket(newClientConnected);

			/************************************************************************/
			/* Send the already networkSynced objects located on the server to the	*/
			/* newly connected client.												*/
			/************************************************************************/ 
			for( unsigned int i=0; i<p_entities.size(); i++ )
			{
				int entityId = p_entities[i]->getIndex();
				netSync = (NetworkSynced*)m_world->getComponentManager()->
					getComponent( entityId, ComponentType::NetworkSynced );
				
				// Create entity
				if( netSync->getNetworkType() == EntityType::Ship ||
					netSync->getNetworkType() == EntityType::Prop
					)
				{				
					transform = (Transform*)m_world->getComponentManager()->
						getComponent( entityId, ComponentType::Transform );

					Packet packet((char)PacketType::EntityCreation);
					packet << (char)netSync->getNetworkType() 
						<< netSync->getNetworkOwner() 
						<< entityId << transform->getTranslation() 
						<< transform->getRotation() << transform->getScale();

					m_server->unicastPacket( packet, id );
				}
			}

		}
	}
}

void ServerListenerSystem::initialize()
{
	m_server->startListening(1337);
}
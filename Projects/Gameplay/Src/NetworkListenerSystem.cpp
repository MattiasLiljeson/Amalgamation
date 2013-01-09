#include "NetworkListenerSystem.h"
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
#include "NetworkType.h"

// Components:
#include "Transform.h"
#include "NetworkSynced.h"
#include "ShipFlyController.h"
#include "BodyInitData.h"
#include "PhysicsBody.h"
#include "PlayerScore.h"

NetworkListenerSystem::NetworkListenerSystem( TcpServer* p_server )
	: EntitySystem( SystemType::NetworkListenerSystem, 1, ComponentType::NetworkSynced)
{
	m_server = p_server;
}

NetworkListenerSystem::~NetworkListenerSystem()
{
	m_server->stopListening();
}

void NetworkListenerSystem::processEntities( const vector<Entity*>& p_entities )
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

			///
			/// Give the new client its Network Identity.
			///
			Packet identityPacket;
			identityPacket << (char)PacketType::InitCredentials <<
				(char)NetworkType::Identity << id;
			m_server->unicastPacket( identityPacket, id );


			// Create a new entity for the connecting client, and belonging components
			Entity* e = m_world->createEntity();

			Transform* transform = new Transform( (float)(id) * 10.0f, 0, 0 );
			NetworkSynced* netSync = 
				new NetworkSynced( e->getIndex(), id, NetworkType::Ship );
			
			e->addComponent( ComponentType::Transform, transform);
			e->addComponent( ComponentType::NetworkSynced, netSync);
			e->addComponent( ComponentType::ShipFlyController,
				new ShipFlyController(5.0f, 50.0f));
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
			Packet newClientConnected;
			newClientConnected << 
				(char)PacketType::EntityCreation <<
				(char)NetworkType::Ship << id << e->getIndex() <<
				transform->getTranslation() << transform->getRotation() <<
				transform->getScale();
			
			m_server->broadcastPacket(newClientConnected);

			//m_server->multicastPacket( currentConnections, newClientConnected );
			
			// The server must then initialize data for the new client.
			// Suggestion
			// Packets needed: CREATE_ENTITY
			//	int:	id
			//	string: name (debug)
			// Packets	needed: ADD_COMPONENT
			//	int:	entityId
			//	int:	componentTypeId
			//	*:		specificComponentData

			// Send the old networkSynced stuff:
			for( unsigned int i=0; i<p_entities.size(); i++ )
			{
				int entityId = p_entities[i]->getIndex();
				netSync = (NetworkSynced*)m_world->getComponentManager()->
					getComponent( entityId, ComponentType::NetworkSynced );
				
				transform = (Transform*)m_world->getComponentManager()->
					getComponent( entityId, ComponentType::Transform );

				// Create entity
				if( netSync->getNetworkType() == NetworkType::Ship ||
					netSync->getNetworkType() == NetworkType::Prop
					)
				{
					Packet packet;
					packet << (char)PacketType::EntityCreation <<
						(char)netSync->getNetworkType() << netSync->getNetworkOwner() << 
						entityId << transform->getTranslation() <<
						transform->getRotation() << transform->getScale();

					m_server->unicastPacket( packet, id );
				}
			}

		}
	}
}

void NetworkListenerSystem::initialize()
{
	m_server->startListening(1337);
}
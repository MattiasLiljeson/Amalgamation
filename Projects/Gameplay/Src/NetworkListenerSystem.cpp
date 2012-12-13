#include "NetworkListenerSystem.h"

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
		Packet dcPacket;
		dcPacket 
			<< (char)PacketType::ClientDisconnect
			<< (char)id; 
			
		m_server->broadcastPacket( Packet() );

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

			// Create a new entity for the connecting client, and belonging components
			Entity* e = m_world->createEntity();

			Transform* transform = new Transform( (float)(id) * 10.0f, 0, 0 );
			NetworkSynced* netSync = 
				new NetworkSynced( e->getIndex(), id, NetworkType::Ship );
			
			e->addComponent( ComponentType::Transform, transform);
			e->addComponent( ComponentType::NetworkSynced, netSync);

			m_world->addEntity( e );

			// When a client is connecting, the server must broadcast to all other
			// clients that a new client exists.
			// Packet needed: ON_CLIENT_CONNECT 
			//	data: clientId
			//	And entity creation.
			
			vector<int> currentConnections = m_server->getActiveConnections();

			for( unsigned int i=0; i<currentConnections.size(); i++ )
			{
				if( currentConnections[i] == id )
				{
					// Removes the new client from this vector.
					currentConnections.erase( currentConnections.begin() + i );
					DEBUGPRINT(( toString(i).c_str() ));
				}
			}

			// HACK: Just some testing packet here.
			// Broadcast the new client's entity to all clients, even the new one.
			Packet newClientConnected;
			newClientConnected << 
				(char)PacketType::EntityCreation <<
				(char)NetworkType::Ship << id << e->getIndex() <<

				(float)(id) * 10.0f << (float)0 << (float)0;
			
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

			// Send the new ship created:
			NetworkSynced* netSync = NULL;
			netSync = (NetworkSynced*)m_world->getComponentManager()->
				getComponent(
					e->getIndex(), ComponentType::NetworkSynced );


			// Send the old networkSynced stuff:
			for( unsigned int i=0; i<p_entities.size(); i++ )
			{
				int entityId = p_entities[i]->getIndex();
				netSync = (NetworkSynced*)m_world->getComponentManager()->
					getComponent( entityId, ComponentType::NetworkSynced );
				
				transform = (Transform*)m_world->getComponentManager()->
					getComponent( entityId, ComponentType::Transform );

				// Create entity
				if( netSync->getNetworkType() == NetworkType::Ship )
				{
					Packet packet;
					packet << (char)PacketType::EntityCreation <<
						(char)NetworkType::Ship << netSync->getNetworkOwner() << 
						entityId << transform->getTranslation();

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
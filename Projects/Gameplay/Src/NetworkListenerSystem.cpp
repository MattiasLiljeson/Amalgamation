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
		for (unsigned int index = 0; index < p_entities.size(); index++)
		{
			NetworkSynced* netSync = static_cast<NetworkSynced*>(
				m_world->getComponentManager()->getComponent( p_entities[index],
				ComponentType::getTypeFor( ComponentType::NetworkSynced ) ) );

			// When a client is disconnecting, then all other clients must know this.

			// HACK: This deletion is what caused the magical crashes all the time.
			// This should be solved as soon as possible.
//			if (netSync->getNetworkIdentity() == id)
//				m_world->deleteEntity(p_entities[index]);
		}
	}





	if ( m_server->isListening() )
	{
		while( m_server->hasNewConnections() )
		{
			int id = m_server->popNewConnection();

			Entity* e = m_world->createEntity();
			e->addComponent( ComponentType::getTypeFor( ComponentType::Transform ),
				new Transform( (float)(id) * 10.0f, 0, 0 ) );
			e->addComponent( ComponentType::getTypeFor( ComponentType::NetworkSynced ),
				new NetworkSynced( id, id ) );
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
					currentConnections.erase( currentConnections.begin() + i );
					DEBUGPRINT(( toString(i).c_str() ));
				}
			}

			// HACK: Just some testing packet here.
			Packet newClientConnected;
			newClientConnected << (char)PacketType::EntityCreation <<
				(char)NetworkType::Ship << id <<
				(float)(id) * 10.0f << (float)0 << (float)0;
			m_server->multicastPacket( currentConnections, newClientConnected );

			
			// The server must then initialize data for the new client.
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
				netSync = (NetworkSynced*)m_world->getComponentManager()->
					getComponent(
						p_entities[i]->getIndex(), ComponentType::NetworkSynced );

				//Packet create

				if( netSync->getNetworkType() == NetworkType::Ship )
				{

				}
			}

		}
	}
}

void NetworkListenerSystem::initialize()
{
	m_server->startListening(1337);
}
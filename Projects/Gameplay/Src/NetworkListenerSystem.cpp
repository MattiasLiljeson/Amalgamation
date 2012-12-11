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

void NetworkListenerSystem::process()
{
	EntitySystem::process();

	if ( m_server->isListening() )
	{
		while( m_server->hasNewConnections() )
		{
			int id = m_server->popNewConnection();

			Entity* e = m_world->createEntity();
			e->addComponent( ComponentType::getTypeFor( ComponentType::Transform ),
				new Transform( (float)(id) * 10.0f, 0, 0 ) );
			e->addComponent( ComponentType::getTypeFor( ComponentType::NetworkSynced ),
				new NetworkSynced( id ) );
			m_world->addEntity( e );

		}
	}
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

			// HACK: This deletion is what caused the magical crashes all the time.
			// This should be solved as soon as possible.
//			if (netSync->getNetworkIdentity() == id)
//				m_world->deleteEntity(p_entities[index]);
		}
	}
}

void NetworkListenerSystem::initialize()
{
	m_server->startListening(1337);
}
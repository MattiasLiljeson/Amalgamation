#include "NetworkListenerSystem.h"

NetworkListenerSystem::NetworkListenerSystem( TcpServer* p_server )
	: EntitySystem( SystemType::NetworkListenerSystem )
{
	m_server = p_server;
}

NetworkListenerSystem::~NetworkListenerSystem()
{
}

void NetworkListenerSystem::process()
{
	while( m_server->hasNewConnections() )
	{
		int id = m_server->popNewConnection();

		Entity* e = m_world->createEntity();
		e->addComponent( ComponentType::getTypeFor( ComponentType::Transform ),
			new Transform( 1.0f, 0, 0 ) );
		e->addComponent( ComponentType::getTypeFor( ComponentType::NetworkSynced ),
			new NetworkSynced( 1 ) );

	}
}

void NetworkListenerSystem::initialize()
{
}
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
	if( m_server->hasNewConnections() )
	{

	}
}

void NetworkListenerSystem::initialize()
{
}
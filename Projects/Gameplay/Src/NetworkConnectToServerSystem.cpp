#include "NetworkConnectToServerSystem.h"

NetworkConnectToServerSystem::NetworkConnectToServerSystem( TcpClient* p_tcpClient )
	: EntitySystem( SystemType::NetworkConnectoToServerSystem )
{
	m_tcpClient = p_tcpClient;
}

NetworkConnectToServerSystem::~NetworkConnectToServerSystem()
{

}

void NetworkConnectToServerSystem::processEntities( const vector<Entity*>& p_entities )
{
	if( !m_tcpClient->hasActiveConnection() )
	{
		m_tcpClient->connectToServer( "194.47.150.135", "1337" );
	}
	else
	{
		m_world->getSystem(SystemType::NetworkCommunicatorSystem)->setEnabled(true);
		setEnabled(false);
	}
}

void NetworkConnectToServerSystem::initialize()
{
}

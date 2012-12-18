#include "NetworkConnectToServerSystem.h"

NetworkConnectToServerSystem::NetworkConnectToServerSystem( TcpClient* p_tcpClient,
												InputBackendSystem* p_inputBackend  )
	: EntitySystem( SystemType::NetworkConnectoToServerSystem )
{
	m_tcpClient = p_tcpClient;
	m_inputBackend = p_inputBackend;
}

NetworkConnectToServerSystem::~NetworkConnectToServerSystem()
{

}

void NetworkConnectToServerSystem::processEntities( const vector<Entity*>& p_entities )
{
	Control* keyL = m_inputBackend->getInputControl("L");
	double pressness = keyL->getDelta(); //pressed = 1.0

	if( !m_tcpClient->hasActiveConnection() )
	{
		m_tcpClient->connectToServer( "127.0.0.1", "1337" );
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

#include "NetworkConnectToServerSystem.h"
#include "Control.h"
#include <TcpClient.h>
#include "InputBackendSystem.h"

NetworkConnectToServerSystem::NetworkConnectToServerSystem( TcpClient* p_tcpClient,
												InputBackendSystem* p_inputBackend  )
	: EntitySystem( SystemType::NetworkConnectoToServerSystem )
{
	m_tcpClient = p_tcpClient;
	m_inputBackend = p_inputBackend;

	m_isLookingForConnection = false;
}

NetworkConnectToServerSystem::~NetworkConnectToServerSystem()
{
}

void NetworkConnectToServerSystem::processEntities( const vector<Entity*>& p_entities )
{

	if( !m_tcpClient->hasActiveConnection() )
	{
		
		Control* keyL = m_inputBackend->getControlByEnum(InputHelper::KEY_L);
		double pressness = keyL->getDelta(); //pressed = 1.0
		bool isPressed = pressness == 1.0;
		if( isPressed && !m_isLookingForConnection )
		{
			m_tcpClient->connectToServerAsync( "127.0.0.1", "1337" );
			m_isLookingForConnection = true;
		}
	}
	else
	{
		m_isLookingForConnection = false;
		m_world->getSystem(SystemType::NetworkCommunicatorSystem)->setEnabled(true);
		setEnabled(false);
	}
}

void NetworkConnectToServerSystem::initialize()
{
}

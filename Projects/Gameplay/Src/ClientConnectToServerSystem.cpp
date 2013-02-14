#include "ClientConnectToServerSystem.h"
#include "Control.h"
#include <TcpClient.h>
#include <Rocket/Core/Event.h>
#include "LibRocketEventManagerSystem.h"
#include "InputBackendSystem.h"
#include "GameState.h"

ClientConnectToServerSystem::ClientConnectToServerSystem(TcpClient* p_tcpClient,
														 bool p_connectDirectly/* =false */)
	: EntitySystem( SystemType::ClientConnectoToServerSystem , 1, ComponentType::GameState), EventHandler("join")
{
	m_tcpClient = p_tcpClient;
	m_connectStraightAway = p_connectDirectly;
	
	
	m_isLookingForConnection = false;
	/*m_connectionAddress.octets1 = 127;
	m_connectionAddress.octets2 = 0;
	m_connectionAddress.octets3 = 0;
	m_connectionAddress.octets4 = 1;
	m_connectionAddress.port =	1337;
	*/

	m_serverAddress = "127.0.0.1";
	m_serverPort = "1337";
}

ClientConnectToServerSystem::~ClientConnectToServerSystem()
{
}

void ClientConnectToServerSystem::initialize()
{
	if(m_connectStraightAway){
		connectToNetworkAddress();
	}
}

void ClientConnectToServerSystem::processEntities( const vector<Entity*>& p_entities )
{
	if( m_tcpClient->hasActiveConnection() )
	{
		m_isLookingForConnection = false;
		m_world->getSystem(SystemType::ClientPacketHandlerSystem)->setEnabled(true);
		setEnabled(false);

		auto eventManagerSys = static_cast<LibRocketEventManagerSystem*>(
			m_world->getSystem(SystemType::LibRocketEventManagerSystem));

		// Clears and hides all currently visible documents.
		eventManagerSys->clearDocumentStack();
	}
	else if(p_entities.size() > 0)
	{
		auto state = static_cast<GameState*>(p_entities[0]->getComponent(
			ComponentType::GameState));

		InputBackendSystem* inputBackend = static_cast<InputBackendSystem*>
			(m_world->getSystem(SystemType::InputBackendSystem));

		if( inputBackend->getDeltaByEnum(InputHelper::KeyboardKeys_F4) > 0 
			|| state->getStateDelta(INGAME)!= 0){
			connectToNetworkAddress();
			state->setStatesDelta(INGAME,1);
		}

	}
}

void ClientConnectToServerSystem::connectToNetworkAddress()
{
	m_tcpClient->connectToServerAsync( m_serverAddress, m_serverPort);
	m_isLookingForConnection = true;
}

/*
void ClientConnectToServerSystem::connectToNetworkAddress(
														const std::string& p_serverAddress, 
														const std::string& p_serverPort)
{
	m_tcpClient->connectToServerAsync( p_serverAddress, p_serverPort);
	m_isLookingForConnection = true;
}
*/

void ClientConnectToServerSystem::processEvent( Rocket::Core::Event& p_event, const Rocket::Core::String& p_value )
{
	// Sent from the 'onsubmit' of the play screen, we set the network ip and port here,
	// and enable the system.
	
}

void ClientConnectToServerSystem::setConnectionAddress( const std::string& p_address, 
													   const std::string& p_port )
{
	m_serverAddress = p_address;
	m_serverPort = p_port;
}

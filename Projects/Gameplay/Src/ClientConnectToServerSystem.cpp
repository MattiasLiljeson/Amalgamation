#include "ClientConnectToServerSystem.h"
#include "Control.h"
#include <TcpClient.h>
#include <Rocket/Core/Event.h>
#include "LibRocketEventManagerSystem.h"
#include "InputBackendSystem.h"

ClientConnectToServerSystem::ClientConnectToServerSystem(TcpClient* p_tcpClient,
														 bool p_connectDirectly/* =false */)
	: EntitySystem( SystemType::ClientConnectoToServerSystem ), EventHandler("join")
{
	m_tcpClient = p_tcpClient;
	m_connectStraightAway = p_connectDirectly;
	
	m_isLookingForConnection = false;
	m_connectionAddress.octets1 = 127;
	m_connectionAddress.octets2 = 0;
	m_connectionAddress.octets3 = 0;
	m_connectionAddress.octets4 = 1;
	m_connectionAddress.port =	1337;
}

ClientConnectToServerSystem::~ClientConnectToServerSystem()
{
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
	else
	{
		InputBackendSystem* inputBackend = static_cast<InputBackendSystem*>(m_world->getSystem(
			SystemType::InputBackendSystem));
		if( inputBackend->getDeltaByEnum(InputHelper::KeyboardKeys_F4) > 0 )
		{
			connectToNetworkAddress();
		}
	}
}

void ClientConnectToServerSystem::initialize()
{
	if(m_connectStraightAway){
		connectToNetworkAddress();
	}
}


void ClientConnectToServerSystem::connectToNetworkAddress()
{
	string address = m_connectionAddress.getIpAddress();
	address = m_connectionAddress.getPortAddress();

	m_tcpClient->connectToServerAsync( m_connectionAddress.getIpAddress(), 
		m_connectionAddress.getPortAddress() );
	m_isLookingForConnection = true;
}

void ClientConnectToServerSystem::connectToNetworkAddress(
														const std::string& p_serverAddress, 
														const std::string& p_serverPort)
{
	m_tcpClient->connectToServerAsync( p_serverAddress, p_serverPort);
	m_isLookingForConnection = true;
}

void ClientConnectToServerSystem::processEvent( Rocket::Core::Event& p_event, const Rocket::Core::String& p_value )
{
	// Sent from the 'onsubmit' of the play screen, we set the network ip and port here,
	// and enable the system.
	if (p_value == "join_server")
	{
		// "server_host" is the name attribute specified in the input element in the rml file.
		// "localhost" simply is provided as a default value, if the host isn't set. This could be left as "" as well.
		string server_host = p_event.GetParameter<Rocket::Core::String>("server_host", "localhost").CString();
		string server_port = p_event.GetParameter<Rocket::Core::String>("server_port", "1337").CString();

		if( !m_tcpClient->hasActiveConnection() &&  !m_isLookingForConnection)
			connectToNetworkAddress(server_host, server_port);
	}
}
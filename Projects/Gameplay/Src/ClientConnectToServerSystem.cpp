#include "ClientConnectToServerSystem.h"
#include "Control.h"
#include <TcpClient.h>
#include <Rocket/Core/Event.h>
#include "LibRocketBackendSystem.h"

ClientConnectToServerSystem* ClientConnectToServerSystem::m_selfPointer = NULL;

ClientConnectToServerSystem::ClientConnectToServerSystem( TcpClient* p_tcpClient )
	: EntitySystem( SystemType::NetworkConnectoToServerSystem ), EventHandler("join")
{
	m_tcpClient = p_tcpClient;
	
	m_isLookingForConnection = false;
	m_connectionAddress.octets1 = 127;
	m_connectionAddress.octets2 = 0;
	m_connectionAddress.octets3 = 0;
	m_connectionAddress.octets4 = 1;
	m_connectionAddress.port =	1337;

	m_selfPointer = this;
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
	}
}

void ClientConnectToServerSystem::initialize()
{
	auto rocketBackend = static_cast<LibRocketBackendSystem*>(
		m_world->getSystem(SystemType::LibRocketBackendSystem));
	rocketBackend->registerEventHandler(this);

	TwStructMember ipMembers[] = {
		{ "Ip-part1", TW_TYPE_INT32, offsetof(NetworkAdress,octets1),"min=0 max=255"},
		{ "Ip-part2", TW_TYPE_INT32, offsetof(NetworkAdress,octets2),"min=0 max=255"},
		{ "Ip-part3", TW_TYPE_INT32, offsetof(NetworkAdress,octets3),"min=0 max=255"},
		{ "Ip-part4", TW_TYPE_INT32, offsetof(NetworkAdress,octets4),"min=0 max=255"},
		{ "Port",	  TW_TYPE_INT32, offsetof(NetworkAdress,port),"min=0"	},
	};

	TwType myStruct;

	myStruct = TwDefineStruct("IP-address", ipMembers,5,sizeof(NetworkAdress),NULL,NULL);
	AntTweakBarWrapper::getInstance()->addWriteVariable(AntTweakBarWrapper::NETWORK,
		"Connection address",myStruct,&m_connectionAddress,"");

	TwAddButton(AntTweakBarWrapper::getInstance()->getAntBar(AntTweakBarWrapper::NETWORK),
		"Connect to address", callbackConnectToNetworkAddress, NULL, "key=F4");
}

void TW_CALL ClientConnectToServerSystem::callbackConnectToNetworkAddress( void* p_clientData )
{
	if( !m_selfPointer->m_tcpClient->hasActiveConnection() 
		&& !m_selfPointer->m_isLookingForConnection)
	{
		m_selfPointer->connectToNetworkAddress();
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

void ClientConnectToServerSystem::ProcessEvent( Rocket::Core::Event& event, const Rocket::Core::String& value )
{
	// Sent from the 'onsubmit' of the play screen, we set the network ip and port here,
	// and enable the system.
	if (value == "join_server")
	{
		// NOTE: Issue!
		// Boost seem to be unable to resolve servers specified by a name (localhost does
		// work). This needs to be researched further. // Alex
		//string server_name		= event.GetParameter<Rocket::Core::String>("server_name", "").CString();
		string server_host = event.GetParameter<Rocket::Core::String>("server_host", "localhost").CString();
		string server_port = event.GetParameter<Rocket::Core::String>("server_port", "1337").CString();

		if( !m_tcpClient->hasActiveConnection() &&  !m_isLookingForConnection)
			connectToNetworkAddress(server_host, server_port);
	}
}

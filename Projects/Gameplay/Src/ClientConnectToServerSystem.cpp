#include "ClientConnectToServerSystem.h"
#include "Control.h"
#include <TcpClient.h>

ClientConnectToServerSystem* ClientConnectToServerSystem::m_selfPointer = NULL;

ClientConnectToServerSystem::ClientConnectToServerSystem( TcpClient* p_tcpClient )
	: EntitySystem( SystemType::NetworkConnectoToServerSystem )
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
		"Connect to address", callbackConnectToNetworkAddress, NULL, "key=CTRL+l");
}

void TW_CALL ClientConnectToServerSystem::callbackConnectToNetworkAddress( void* p_clientData )
{
	m_selfPointer->connectToNetworkAddress();
}

void ClientConnectToServerSystem::connectToNetworkAddress()
{
	m_tcpClient->connectToServerAsync( m_connectionAddress.getIpAddress(), 
		m_connectionAddress.getPortAddress() );
	m_isLookingForConnection = true;
}
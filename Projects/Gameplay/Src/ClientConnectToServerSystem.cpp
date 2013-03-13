#include "ClientConnectToServerSystem.h"

#include <boost/asio/ip/v6_only.hpp>
#include <boost/asio/ip/address.hpp>
#include "Control.h"
#include <TcpClient.h>
#include "InputBackendSystem.h"
#include "LevelHandlerSystem.h"
#include "GameState.h"
#include "ClientStateSystem.h"
#include <DebugUtil.h>

ClientConnectToServerSystem::ClientConnectToServerSystem(TcpClient* p_tcpClient,
														 bool p_connectDirectly/* =false */)
	: EntitySystem( SystemType::ClientConnectoToServerSystem )
{
	m_tcpClient = p_tcpClient;
	m_connectStraightAway = p_connectDirectly;
	
	m_isLookingForConnection = false;

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

void ClientConnectToServerSystem::process()
{
	if( m_tcpClient->hasActiveConnection() )
	{
		m_isLookingForConnection = false;
		//m_world->getSystem(SystemType::ClientPacketHandlerSystem)->setEnabled(true);
		setEnabled(false);
	}

}

void ClientConnectToServerSystem::connectToNetworkAddress()
{
	m_tcpClient->connectToServerAsync( m_serverAddress, m_serverPort);
	m_isLookingForConnection = true;
}


std::string ClientConnectToServerSystem::getServerAddress()
{
	return m_serverAddress;
}

std::string ClientConnectToServerSystem::getServerPort()
{
	return m_serverPort;
}

bool ClientConnectToServerSystem::setAddressAndConnect( const std::string& p_address, 
													   const std::string& p_port )
{
	if(setConnectionAddress(p_address,p_port)){
		connectToNetworkAddress();
		return true;
	}
	return false;
}

bool ClientConnectToServerSystem::setConnectionAddress( const std::string& p_address, 
													   const std::string& p_port )
{
	if(validateNetworkAddress(p_address, p_port)){
		m_serverAddress = p_address;
		m_serverPort = p_port;
		return true;
	}
	return false;
}

bool ClientConnectToServerSystem::validateNetworkAddress( const std::string& p_address, 
														 const std::string& p_port )
{
	if(validateIPFormat(p_address) && validatePortFormat(p_port)){
		return true;
	}

	return false;
}

bool ClientConnectToServerSystem::validateIPFormat(const std::string& p_address)
{
	boost::asio::ip::address ipAddress;
	try{
		ipAddress = boost::asio::ip::address::from_string(p_address);
	}
	catch(const std::exception& e){
		DEBUGPRINT(("Invalid IP address submitted\n"));
		return false;
	}
	return true;
}

bool ClientConnectToServerSystem::validatePortFormat( const std::string& p_port )
{
	istringstream buffer(p_port);
	int portValue;
	try{
		buffer >> portValue;
	}
	catch(exception& e){
		DEBUGPRINT(("Invalid Port address submitted\n"));
		return false;
	}

	if(portValue < 1024 || portValue > 65535){
		DEBUGPRINT(("Invalid Port range\n"));
		return false;
	}

	return true;
}


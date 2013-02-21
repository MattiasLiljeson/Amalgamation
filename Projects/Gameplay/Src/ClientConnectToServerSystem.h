#pragma once

#include <EntitySystem.h>
#include <string>
#include <ToString.h>

using namespace std;

class TcpClient;

// =======================================================================================
//	ClientConnectToServerSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief description...
///        
/// # NetworkConnectToServerSystem
/// Detailed description...
/// Created on: 11-12-2012 
///---------------------------------------------------------------------------------------

class ClientConnectToServerSystem: public EntitySystem
{
public:
	ClientConnectToServerSystem( TcpClient* p_tcpClient, bool p_connectDirectly=false);

	~ClientConnectToServerSystem();

	virtual void process();

	virtual void initialize();

	void connectToNetworkAddress();
	//void connectToNetworkAddress(const std::string& p_serverAddress, const std::string& p_serverPort);

	void setConnectionAddress(const std::string& p_address, const std::string& p_port);
private:
	TcpClient* m_tcpClient;

	bool	m_isLookingForConnection;
	bool	m_connectStraightAway;

	string m_serverAddress;
	string m_serverPort;
};
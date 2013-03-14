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

	bool setAddressAndConnect(const std::string& p_address, const std::string& p_port);
	bool setConnectionAddress(const std::string& p_address, const std::string& p_port);

	string getServerAddress();
	string getServerPort();
	int getServerPortByInt();
private:
	bool validateNetworkAddress(const std::string& p_address, const std::string& p_port);
	bool validateIPFormat(const std::string& p_address);
	bool validatePortFormat(const std::string& p_port);
private:
	TcpClient* m_tcpClient;

	bool	m_isLookingForConnection;
	bool	m_connectStraightAway;

	string m_serverAddress;
	string m_serverPort;
};
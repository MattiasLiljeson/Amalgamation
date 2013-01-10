#pragma once

#include <AglVector4.h>
#include <EntitySystem.h>
#include <AntTweakBarWrapper.h>
#include <string>

using namespace std;

class InputBackendSystem;
class TcpClient;

struct NetworkAdress
{
	int octets1;
	int octets2;
	int octets3;
	int octets4;

	int port;

	string getIpAddress()
	{
		string tmp;
		tmp = octets1;
		tmp += octets2;
		tmp += octets3;
		tmp += octets4;

		return tmp;
	}

	string getPortAddress()
	{
		string tmp;
		tmp = port;
		return tmp;
	}
};

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
	ClientConnectToServerSystem( TcpClient* p_tcpClient,  
		InputBackendSystem* p_inputBackend  );

	~ClientConnectToServerSystem();

	virtual void processEntities( const vector<Entity*>& p_entities );

	virtual void initialize();

	void connectToNetworkAddress();

private:
	TcpClient* m_tcpClient;
	InputBackendSystem* m_inputBackend;

	bool	m_isLookingForConnection;
	NetworkAdress m_connectionAddress;

	/************************************************************************/
	/* DEBUG FUNCTIONS ONLY! */
	/************************************************************************/
	static ClientConnectToServerSystem* m_selfPointer;
private:
	static void TW_CALL callbackConnectToNetworkAddress(void* p_clientData);
};
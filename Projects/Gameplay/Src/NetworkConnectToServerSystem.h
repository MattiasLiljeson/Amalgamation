#pragma once

#include <EntitySystem.h>

class InputBackendSystem;
class TcpClient;

// =======================================================================================
//	NetworkConnectToServerSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief description...
///        
/// # NetworkConnectToServerSystem
/// Detailed description...
/// Created on: 11-12-2012 
///---------------------------------------------------------------------------------------

class NetworkConnectToServerSystem: public EntitySystem
{
public:
	NetworkConnectToServerSystem( TcpClient* p_tcpClient,  InputBackendSystem* p_inputBackend  );

	~NetworkConnectToServerSystem();

	virtual void processEntities( const vector<Entity*>& p_entities );

	virtual void initialize();

private:
	TcpClient* m_tcpClient;
	InputBackendSystem* m_inputBackend;

	bool m_isLookingForConnection;
};
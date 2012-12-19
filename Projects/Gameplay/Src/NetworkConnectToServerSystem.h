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
#pragma once

#include <TcpClient.h>
#include <EntitySystem.h>
#include "InputBackendSystem.h"

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
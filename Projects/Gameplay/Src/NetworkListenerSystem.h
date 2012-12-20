#pragma once

#include <EntitySystem.h>

class TcpServer;

// =======================================================================================
//                                      NetworkListenerSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Using the TcpListenerProcess to listen for new connections.
///        
/// # NetworkListenerSystem
/// NetworkListenerSystem inherits the ThreadSafeMessaging which enables it to
/// communication thread safe with other processes.
/// It handles new incoming connections.
/// Created on: 3-12-2012 
///---------------------------------------------------------------------------------------


class NetworkListenerSystem: public EntitySystem
{
public:
	NetworkListenerSystem( TcpServer* p_server );
	~NetworkListenerSystem();

	virtual void processEntities(const vector<Entity*>& p_entities );

	virtual void initialize();

private:
	TcpServer* m_server;

};
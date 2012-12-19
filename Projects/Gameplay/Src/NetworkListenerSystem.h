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
#pragma once

#include <iostream>
#include <queue>
#include <boost/asio.hpp>
#include <vector>

#include <SystemType.h>
#include <EntitySystem.h>
#include <ProcessMessageTerminate.h>
#include <ThreadSafeMessaging.h>
#include <TcpListenerProcess.h>
#include <TcpServer.h>
#include <DebugUtil.h>
#include <ToString.h>

#include "PacketType.h"
#include "NetworkType.h"

// Components:
#include "Transform.h"
#include "NetworkSynced.h"
#include "ShipController.h"
#include "BodyInitData.h"
#include "PhysicsBody.h"

using namespace std;
using namespace boost::asio::ip;


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
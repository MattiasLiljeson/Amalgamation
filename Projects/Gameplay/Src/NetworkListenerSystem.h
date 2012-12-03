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

#include <SystemType.h>
#include <EntitySystem.h>
#include <ProcessMessageTerminate.h>
#include <ThreadSafeMessaging.h>
#include <TcpListenerProcess.h>

using namespace std;
using namespace boost::asio::ip;


class NetworkListenerSystem: public EntitySystem, public ThreadSafeMessaging
{
public:
	NetworkListenerSystem();
	~NetworkListenerSystem();

	virtual void process();

	virtual void initialize();

private:
	boost::asio::io_service* m_ioService;

	ProcessThread* m_tcpListenerProcess;
	ThreadSafeMessaging* m_tcpListenerProcessMessaging;

};
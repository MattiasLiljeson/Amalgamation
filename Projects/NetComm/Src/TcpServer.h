// =======================================================================================
//                                      TcpServer
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief description...
///        
/// # TcpServer
/// Detailed description...
/// Created on: 4-12-2012 
///---------------------------------------------------------------------------------------
#pragma once

#include <queue>

#include <boost/asio.hpp>

#include "ThreadSafeMessaging.h"
#include "TcpListenerProcess.h"
#include "ProcessMessageClientConnected.h"
#include "ProcessMessageReceivePacket.h"

using namespace std;
using namespace boost::asio::ip;

class TcpServer: public ThreadSafeMessaging
{
public:
	TcpServer();
	~TcpServer();

	void startListening( int p_port );

	void stopListening();

	bool isListening();

	bool hasNewConnections();

	bool hasNewPackets();

	void processMessages();

private:
	bool m_isListening;

	queue< ProcessMessageClientConnected* > m_newConnections;
	queue< ProcessMessageReceivePacket* > m_newPackets;

	TcpListenerProcess* m_listenerProcess;
	boost::asio::io_service* m_ioService;

};
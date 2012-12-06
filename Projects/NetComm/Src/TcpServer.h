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
#include <vector>
#include <iostream>

#include <boost/asio.hpp>

#include "ThreadSafeMessaging.h"
#include "TcpListenerProcess.h"
#include "TcpCommunicationProcess.h"
#include "ProcessMessageClientConnected.h"
#include "ProcessMessageReceivePacket.h"
#include "StringSplit.h"
#include "Packet.h"

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
	unsigned int newConnectionsCount();

	unsigned int activeConnectionsCount();

	unsigned int newDisconnectionsCount();

	bool hasNewPackets();
	unsigned int newPacketsCount();
	Packet* popNewPacket();

	void processMessages();

private:
	bool m_isListening;

	queue< int > m_newConnectionProcesses;
	queue< int > m_newDisconnectionProcesses;
	vector< TcpCommunicationProcess* > m_communicationProcesses;

	queue< Packet* > m_newPackets;

	TcpListenerProcess* m_listenerProcess;
	boost::asio::io_service* m_ioService;

};
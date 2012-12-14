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
#include <exception>

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
	int popNewConnection();

	unsigned int activeConnectionsCount();
	vector< int > getActiveConnections();

	bool hasNewDisconnections();
	unsigned int newDisconnectionsCount();
	int popNewDisconnection();

	bool hasNewPackets();
	unsigned int newPacketsCount();
	Packet popNewPacket();

	void processMessages();

	/// Broadcasts a packet to all connected clients
	void broadcastPacket( Packet p_packet );

	void multicastPacket( vector<int> p_connectionIdentities, Packet p_packet );

	/// Unicast sends a packet to only one specified client
	void unicastPacket( Packet p_packet, int clientId );

private:
	bool m_isListening;

	queue< int > m_newConnectionProcesses;
	queue< int > m_newDisconnectionProcesses;
	vector< TcpCommunicationProcess* > m_communicationProcesses;

	queue< Packet > m_newPackets;

	TcpListenerProcess* m_listenerProcess;
	boost::asio::io_service* m_ioService;

};
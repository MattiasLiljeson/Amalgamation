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

#include "ThreadSafeMessaging.h"
#include "Packet.h"

namespace boost
{
	namespace asio
	{
		class io_service;
	};
};

using namespace std;

class TcpCommunicationProcess;
class TcpListenerProcess;

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
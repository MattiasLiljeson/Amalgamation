// =======================================================================================
//                                      TcpClient
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief description...
///        
/// # TcpClient
/// Detailed description...
/// Created on: 4-12-2012 
///---------------------------------------------------------------------------------------
#pragma once

#include <string>
#include <exception>

#include <boost/asio.hpp>

#include "Packet.h"
#include "TcpCommunicationProcess.h"
#include "ThreadSafeMessaging.h"

using namespace std;
using namespace boost::asio::ip;

class TcpClient: public ThreadSafeMessaging
{
public:
	TcpClient();
	~TcpClient();

	bool connectToServer( string p_address, string p_port );

	void disconnect();

	bool hasActiveConnection();

	bool hasNewPackets();
	unsigned int newPacketsCount();
	Packet popNewPacket();

	void sendPacket( Packet p_packet );

	void processMessages();

	int getId();
	int* getIdPointer();
	void setId( int p_id );

private:
	boost::asio::io_service* m_ioService;

	int m_numConnections;
	TcpCommunicationProcess* m_communicationProcess;

	queue< Packet > m_newPackets;

	int m_id;

};
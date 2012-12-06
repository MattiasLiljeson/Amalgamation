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

	void sendPacket( Packet* p_packet );

private:
	boost::asio::io_service* m_ioService;

	int m_numConnections;

	TcpCommunicationProcess* m_communicationProcess;

};
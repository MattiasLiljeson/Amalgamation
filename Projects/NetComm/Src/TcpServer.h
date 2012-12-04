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

#include <boost/asio.hpp>

#include "ThreadSafeMessaging.h"
#include "TcpListenerProcess.h"

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

private:
	bool m_isListening;

	TcpListenerProcess* m_listenerProcess;
	boost::asio::io_service* m_ioService;

};
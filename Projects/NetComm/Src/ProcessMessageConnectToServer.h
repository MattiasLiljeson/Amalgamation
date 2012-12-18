// =======================================================================================
//	ProcessMessageConnectToServer
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief description...
///        
/// # ProcessMessageConnectToServer
/// Detailed description...
/// Created on: 18-12-2012 
///---------------------------------------------------------------------------------------
#pragma once

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/basic_stream_socket.hpp>
#include "ProcessMessage.h"

class ProcessMessageConnectToServer: public ProcessMessage
{
public:
	ProcessMessageConnectToServer();

	ProcessMessageConnectToServer( ThreadSafeMessaging* p_sender,
		boost::asio::ip::tcp::socket* p_socket );

	~ProcessMessageConnectToServer();

public:
	boost::asio::ip::tcp::socket* socket;

};
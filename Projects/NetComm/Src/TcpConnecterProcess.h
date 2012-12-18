// =======================================================================================
//	TcpConnecterProcess
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief description...
///        
/// # TcpConnecterProcess
/// Detailed description...
/// Created on: 18-12-2012 
///---------------------------------------------------------------------------------------
#pragma once

#include <string>

#include "ProcessThread.h"
#include "ThreadSafeMessaging.h"

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/basic_resolver.hpp>
#include <boost/asio/ip/basic_resolver_query.hpp>
#include <boost/asio/ip/basic_resolver_iterator.hpp>
#include <boost/asio/basic_stream_socket.hpp>
#include <boost/system/error_code.hpp>

using namespace std;
using namespace boost::asio::ip;

class boost::asio::io_service;

class TcpConnecterProcess: public ProcessThread
{
public:
	TcpConnecterProcess( ThreadSafeMessaging* p_parent,
		boost::asio::io_service* p_ioService, string p_address, string p_port );

	~TcpConnecterProcess();

	virtual void body();

	virtual void processMessages();
	
private:
	ThreadSafeMessaging* m_parent;
	boost::asio::io_service* m_ioService;

	string m_address;
	string m_port;

};
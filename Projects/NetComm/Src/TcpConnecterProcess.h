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

using namespace std;

namespace boost
{
	namespace asio
	{
		class io_service;
	};
};

class ThreadSafeMessaging;


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

	bool m_running;
};
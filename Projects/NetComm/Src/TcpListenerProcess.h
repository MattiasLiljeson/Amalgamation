#pragma once

#include <boost/asio.hpp>

#include "ThreadSafeMessaging.h"
#include "ProcessThread.h"
#include "ProcessMessageClientConnected.h"

using namespace boost::asio::ip;
using namespace std;

class TcpListenerProcess: public ProcessThread
{
public:
	TcpListenerProcess( ThreadSafeMessaging* p_parent, int p_port,
		boost::asio::io_service* p_ioService );

	~TcpListenerProcess();

	///-----------------------------------------------------------------------------------
	/// Overridden from ProcessThread. The method is called from a new thread
	/// when ProcessThread's start() method is called.
	/// \returns void
	///-----------------------------------------------------------------------------------
	void body();

private:
	///-----------------------------------------------------------------------------------
	/// Start accepting for connections on a socket.
	/// \returns void
	///-----------------------------------------------------------------------------------
	void startAccept();

	///-----------------------------------------------------------------------------------
	/// Callback function that handles a new connection.
	/// \param const boost::system_time::error_code & p_error
	/// \returns void
	///-----------------------------------------------------------------------------------
	void handleAccept( const boost::system::error_code& p_error );

private:
	bool m_running;
	ThreadSafeMessaging* m_parent;

	int m_port;
	boost::asio::io_service* m_ioService;
	tcp::acceptor* m_acceptor;
	tcp::socket* m_socket;

};
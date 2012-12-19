#pragma once



#include "ThreadSafeMessaging.h"
#include "ProcessThread.h"
#include "ProcessMessageClientConnected.h"

namespace boost
{
	namespace asio
	{
		class io_service;
	};
};

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
	/// \return void
	///-----------------------------------------------------------------------------------
	void body();

	void processMessages();

private:
	///-----------------------------------------------------------------------------------
	/// Start accepting for connections on a socket.
	/// \return void
	///-----------------------------------------------------------------------------------
	void startAccept();

	///-----------------------------------------------------------------------------------
	/// Callback function that handles a new connection.
	/// \param const boost::system_time::error_code & p_error
	/// \return void
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
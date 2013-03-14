#include "TcpListenerProcess.h"

#include <boost/thread/thread.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/placeholders.hpp>

TcpListenerProcess::TcpListenerProcess( ThreadSafeMessaging* p_parent, int p_port,
									   boost::asio::io_service* p_ioService )
{
	m_running = false;
	m_parent = p_parent;

	m_port = p_port;
	m_ioService = p_ioService;
	m_socket = NULL;

	m_acceptor = NULL;
	m_acceptor = new tcp::acceptor( *m_ioService, tcp::endpoint(
		tcp::v4(), p_port ) );
}

TcpListenerProcess::~TcpListenerProcess()
{
	if( m_acceptor )
		delete m_acceptor;

	if( m_socket )
		delete m_socket;

	m_ioService->stop();
}

void TcpListenerProcess::body()
{
	m_running = true;

	// Starts a callback function that is called on new connection.
	startAccept();

	while( m_running )
	{
		boost::this_thread::sleep( boost::posix_time::millisec(1) );

		// Poll for a new connection. (This is what calls the handleAccept method).
		m_ioService->poll();

		processMessages();

	}
}

void TcpListenerProcess::processMessages()
{
	queue< ProcessMessage* > messages;
	messages = checkoutMessageQueue();

	while( messages.size() > 0 )
	{
		ProcessMessage* msg = messages.front();
		messages.pop();

		if( msg->type == MessageType::TERMINATE )
			m_running = false;

		delete msg;
	}
}

void TcpListenerProcess::startAccept()
{
	m_ioService->reset();

	m_socket = new tcp::socket( *m_ioService );

	tcp::acceptor::non_blocking_io nonBlocking( true );
	m_acceptor->io_control( nonBlocking );

	m_acceptor->async_accept( *m_socket,
		boost::bind( &TcpListenerProcess::handleAccept, this,
		boost::asio::placeholders::error ) );
}

void TcpListenerProcess::handleAccept( const boost::system::error_code& p_error )
{
	if( !p_error )
	{
		m_parent->putMessage( new ProcessMessageClientConnected(
			this, m_socket ) );

		m_socket = NULL;

		// After a new connection was accepted, let's start listening again!
		startAccept();
	}
}

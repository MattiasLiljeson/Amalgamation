#include "TcpConnecterProcess.h"

#include "ThreadSafeMessaging.h"
#include "ProcessMessageConnectToServer.h"
#include "ProcessMessageTerminate.h"
#include <DebugUtil.h>

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/thread/thread.hpp>

using namespace boost::asio::ip;

TcpConnecterProcess::TcpConnecterProcess( ThreadSafeMessaging* p_parent,
		boost::asio::io_service* p_ioService, string p_address, string p_port )
{
	m_parent = p_parent;
	m_ioService = p_ioService;

	m_address = p_address;
	m_port = p_port;

	m_running = false;
}

TcpConnecterProcess::~TcpConnecterProcess()
{
}

void TcpConnecterProcess::body()
{
	m_running = true;

	// resolver:
	tcp::resolver tcpResolver( *m_ioService );
	tcp::resolver::query tcpQuery( m_address, m_port );

	tcp::resolver::iterator endpointIterator;
	tcp::resolver::iterator end;

	endpointIterator = tcpResolver.resolve( tcpQuery );
	end = tcp::resolver::iterator();


	// socket:
	tcp::socket* activeSocket = new tcp::socket( *m_ioService );

	boost::system::error_code error;

	// The client will try to connect until a response from the server has been done, with a timeout.
	while (m_running)
	{

		DEBUGPRINT(("Attempting to connect to server...\n"));
		error = boost::asio::error::host_not_found;
		endpointIterator = tcpResolver.resolve( tcpQuery );

		// Iterate and attempt to connect to resolved endpoints. 
		while( error && endpointIterator != end )
		{
			activeSocket->close();
			activeSocket->connect( *endpointIterator, error );

			*endpointIterator++;
		}

		// When the resolver is finished, let's see if we found a connection.
		if( error )
		{
			//cout << error.message() << endl;
			DEBUGPRINT(((error.message()+"\n").c_str()));
		}
		else
		{
			DEBUGPRINT(("Connection established!\n"));
			m_parent->putMessage( new ProcessMessageConnectToServer( this, activeSocket ) );
			m_running = false;
		}

		processMessages();
	}

	if (error)
	{
		delete activeSocket;
		activeSocket = NULL;
	}
}

void TcpConnecterProcess::processMessages()
{
	queue< ProcessMessage* > messages;
	messages = checkoutMessageQueue();

	while( messages.size() > 0 )
	{
		ProcessMessage* msg = messages.front();
		messages.pop();

		boost::this_thread::sleep(boost::posix_time::milliseconds(1));

		if( msg->type = MessageType::TERMINATE )
			m_running = false;

		delete msg;
	}
}

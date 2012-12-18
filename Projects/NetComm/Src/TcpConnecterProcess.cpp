#include "TcpConnecterProcess.h"

#include "ProcessMessageConnectToServer.h"

TcpConnecterProcess::TcpConnecterProcess( ThreadSafeMessaging* p_parent,
		boost::asio::io_service* p_ioService, string p_address, string p_port )
{
	m_parent = p_parent;
	m_ioService = p_ioService;

	m_address = p_address;
	m_port = p_port;
}

TcpConnecterProcess::~TcpConnecterProcess()
{
}

void TcpConnecterProcess::body()
{
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
	error = boost::asio::error::host_not_found;

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
		cout << error.message() << endl;
		
		delete activeSocket;
		activeSocket = NULL;
	}
	else
	{
		m_parent->putMessage( new ProcessMessageConnectToServer( this, activeSocket ) );
	}
}

void TcpConnecterProcess::processMessages()
{

}

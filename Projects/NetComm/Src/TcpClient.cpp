#include "TcpClient.h"

TcpClient::TcpClient()
{
	m_ioService = new boost::asio::io_service();
	m_activeSocket = NULL;
	m_numConnections = 0;
}

TcpClient::~TcpClient()
{
	delete m_ioService;
}

bool TcpClient::connectToServer( string p_adress, string p_port )
{
	bool success = false;

	// resolver:
	tcp::resolver tcpResolver( *m_ioService );
	tcp::resolver::query tcpQuery( p_adress, p_port );

	tcp::resolver::iterator endpointIterator;
	tcp::resolver::iterator end;

	endpointIterator = tcpResolver.resolve( tcpQuery );
	end = tcp::resolver::iterator();


	// socket:
	m_activeSocket = new tcp::socket( *m_ioService );


	boost::system::error_code error;
	error = boost::asio::error::host_not_found;

	// iterate and attempt to connect to resolved endpoints
	while( error && endpointIterator != end )
	{
		m_activeSocket->close();
		m_activeSocket->connect( *endpointIterator, error );

		*endpointIterator++;
	}

	if( error )
	{
		cout << error.message() << endl;
		
		delete m_activeSocket;
		m_activeSocket = NULL;

		success = false;

//		throw boost::system::system_error( error );
	}
	else
	{
		tcp::no_delay option( true );
		m_activeSocket->set_option( option );

		tcp::socket::non_blocking_io nonBlocking( true );
		m_activeSocket->io_control( nonBlocking );

		m_numConnections += 1;

		success = true;

		// HACK: Should pass the socket to its CommunicationProcess.
		delete m_activeSocket;
		m_activeSocket = NULL;

//		m_messengerProcess = new TcpMessengerProcess(
//			this, activeSocket, m_ioService );
//		m_messengerProcess->start();

	}

	return success;
}

bool TcpClient::hasActiveConnection()
{
	bool hasActive;
	hasActive = (m_numConnections > 0);

	return hasActive;
}

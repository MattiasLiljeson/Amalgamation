#include "TcpClient.h"

TcpClient::TcpClient()
{
	m_ioService = new boost::asio::io_service();
}

TcpClient::~TcpClient()
{
}

bool TcpClient::connectToServer( string p_adress, string p_port )
{
	// resolver:
	tcp::resolver tcpResolver( *m_ioService );
	tcp::resolver::query tcpQuery( p_adress, p_port );

	tcp::resolver::iterator endpointIterator;
	tcp::resolver::iterator end;

	endpointIterator = tcpResolver.resolve( tcpQuery );
	end = tcp::resolver::iterator();


	// socket:
	tcp::socket* activeSocket = new tcp::socket( *m_ioService );


	boost::system::error_code error;
	error = boost::asio::error::host_not_found;

	// iterate and attempt to connect to resolved endpoints
	while( error && endpointIterator != end )
	{
		activeSocket->close();
		activeSocket->connect( *endpointIterator, error );

		*endpointIterator++;
	}

	if( error )
	{
		cout << error.message() << endl;
		cin.get();
		return false;

//		throw boost::system::system_error( error );
	}
	else
	{
		tcp::no_delay option( true );
		activeSocket->set_option( option );

		tcp::socket::non_blocking_io nonBlocking( true );
		activeSocket->io_control( nonBlocking );

//		m_messengerProcess = new TcpMessengerProcess(
//			this, activeSocket, m_ioService );
//		m_messengerProcess->start();

	}
}

bool TcpClient::hasActiveConnection()
{
	return false;
}

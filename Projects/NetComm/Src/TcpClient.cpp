#include "TcpClient.h"

TcpClient::TcpClient()
{
	m_ioService = new boost::asio::io_service();
	m_numConnections = 0;
	m_communicationProcess = NULL;
}

TcpClient::~TcpClient()
{
	if( m_communicationProcess )
	{
		m_communicationProcess->putMessage( new ProcessMessageTerminate() );
		m_communicationProcess->stop();
		delete m_communicationProcess;
	}

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
		
		delete activeSocket;

		success = false;

//		throw boost::system::system_error( error );
	}
	else
	{
//		tcp::no_delay option( true );
//		activeSocket->set_option( option );
//
//		tcp::socket::non_blocking_io nonBlocking( true );
//		activeSocket->io_control( nonBlocking );

		m_numConnections += 1;

		success = true;

		m_communicationProcess = new TcpCommunicationProcess( this, activeSocket,
			m_ioService );
		m_communicationProcess->start();

	}

	return success;
}

void TcpClient::processMessages()
{
	while( getMessageCount() > 0 )
	{
		ProcessMessage* message;
		message = this->popMessage();

		if( message->type == MessageType::RECEIVE_PACKET )
		{
			m_newPackets.push(
				static_cast< ProcessMessageReceivePacket* >(message)->packet );
			cout << "TcpServer, receive: " <<
				m_newPackets.back()->getMessage() << endl;
		}

		delete message;
	}
}

void TcpClient::disconnect()
{
	if( m_communicationProcess )
	{
		m_communicationProcess->putMessage( new ProcessMessageTerminate() );
		m_communicationProcess->stop();
		delete m_communicationProcess;
		m_communicationProcess = NULL;
	}
}

bool TcpClient::hasActiveConnection()
{
	return m_communicationProcess != NULL;
}

void TcpClient::sendPacket( Packet* p_packet )
{
	m_communicationProcess->putMessage( new ProcessMessageSendPacket( this, p_packet ) );
}

bool TcpClient::hasNewPackets()
{
	return !m_newPackets.empty();
}

unsigned int TcpClient::newPacketsCount()
{
	return m_newPackets.size();
}

Packet* TcpClient::popNewPacket()
{
	Packet* packet = NULL;
	packet = m_newPackets.front();
	m_newPackets.pop();

	return packet;
}

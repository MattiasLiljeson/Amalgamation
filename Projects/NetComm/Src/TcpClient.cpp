#include <boost/asio/ip/tcp.hpp>
#include "TcpClient.h"

#include <exception>
#include <iostream>

#include "ProcessMessageConnectToServer.h"
#include "ProcessMessageReceivePacket.h"
#include "ProcessMessageSendPacket.h"
#include "ProcessMessageTerminate.h"

#include "TcpCommunicationProcess.h"

TcpClient::TcpClient()
{
	m_ioService = new boost::asio::io_service();
	m_numConnections = 0;
	m_communicationProcess = NULL;
	m_connecterProcess = NULL;
	m_id = -1;
	m_numberOfSentPackets = 0;
	m_numberOfReceivedPackets = 0;
	m_totalDataSent = 0;
	m_totalDataReceived = 0;
}

TcpClient::~TcpClient()
{
	if( m_communicationProcess )
	{
		m_communicationProcess->putMessage( new ProcessMessageTerminate() );
		m_communicationProcess->stop();
		delete m_communicationProcess;
	}

	if( m_connecterProcess )
	{
		m_connecterProcess->putMessage( new ProcessMessageTerminate() );
		m_connecterProcess->stop();
		delete m_connecterProcess;
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
		std::cout << error.message() << std::endl;
		
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

void TcpClient::connectToServerAsync( string p_address, string p_port )
{
	if( m_connecterProcess == NULL )
	{
		m_connecterProcess = new TcpConnecterProcess( this, m_ioService,
													p_address, p_port );
		m_connecterProcess->start();
	}
}

void TcpClient::processMessages()
{
	queue< ProcessMessage* > messages;
	messages = checkoutMessageQueue();

	while( messages.size() > 0 )
	{
		ProcessMessage* message = messages.front();
		messages.pop();

		if( message->type == MessageType::RECEIVE_PACKET )
		{
			m_newPackets.push(
				static_cast< ProcessMessageReceivePacket* >(message)->packet );
		}
		else if( message->type == MessageType::CONNECT_TO_SERVER )
		{
			ProcessMessageConnectToServer* connectToServerMessage = NULL;
			connectToServerMessage = static_cast<ProcessMessageConnectToServer*>(
				message );

			if( connectToServerMessage->socket )
			{
				m_numConnections += 1;

				m_communicationProcess = new TcpCommunicationProcess( this,
					connectToServerMessage->socket, m_ioService );
				m_communicationProcess->start();

				// Delete the connecter process
				m_connecterProcess->stop();
				delete m_connecterProcess;
				m_connecterProcess = NULL;
			}
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

void TcpClient::sendPacket( Packet p_packet )
{
	if (m_communicationProcess)
	{
		m_numberOfSentPackets += 1;
		m_totalDataSent += p_packet.getDataSize() + 1;

		m_communicationProcess->putMessage(
			new ProcessMessageSendPacket( this, p_packet ) );
	}
}

bool TcpClient::hasNewPackets()
{
	return !m_newPackets.empty();
}

unsigned int TcpClient::newPacketsCount()
{
	return m_newPackets.size();
}

Packet TcpClient::popNewPacket()
{
	if ( !m_newPackets.empty() )
	{	
		Packet packet = m_newPackets.front();
		m_newPackets.pop();

		m_numberOfReceivedPackets += 1;
		m_totalDataReceived += packet.getDataSize() + 1;

		return packet;
	}
	else
	{
		throw domain_error( "Trying to pop from an empty packet queue!" );
	}
	return NULL;
}

int TcpClient::getId()
{
	return m_id;
}

int* TcpClient::getIdPointer()
{
	return &m_id;
}

void TcpClient::setId( int p_id )
{
	m_id = p_id;
}

unsigned int TcpClient::getNumberOfSentPackets()
{
	return m_numberOfSentPackets;
}

void TcpClient::resetNumberOfSentPackets()
{
	m_numberOfSentPackets = 0;
}

unsigned int TcpClient::getNumberOfReceivedPackets()
{
	return m_numberOfReceivedPackets;
}

void TcpClient::resetNumberOfReceivedPackets()
{
	m_numberOfReceivedPackets = 0;
}

unsigned int TcpClient::getTotalDataSent()
{
	return m_totalDataSent;
}

unsigned int TcpClient::getTotalDataReceived()
{
	return m_totalDataReceived;
}

void TcpClient::resetTotalDataSent()
{
	m_totalDataSent = 0;
}

void TcpClient::resetTotalDataReceived()
{
	m_totalDataReceived = 0;
}

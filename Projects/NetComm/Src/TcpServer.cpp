#include "TcpServer.h"

TcpServer::TcpServer()
{
	m_isListening = false;
	m_ioService = new boost::asio::io_service();
	m_listenerProcess = NULL;
}

TcpServer::~TcpServer()
{
	stopListening();

	delete m_ioService;
}

void TcpServer::startListening( int p_port )
{
	m_isListening = true;
	m_listenerProcess = new TcpListenerProcess( this, p_port, m_ioService );
	m_listenerProcess->start();
}

void TcpServer::stopListening()
{
	m_isListening = false;
	if( m_listenerProcess )
	{
		m_listenerProcess->putMessage( new ProcessMessageTerminate( this ) );
		m_listenerProcess->stop();
		delete m_listenerProcess;
		m_listenerProcess = NULL;
	}
}

bool TcpServer::isListening()
{
	return m_isListening;
}

bool TcpServer::hasNewConnections()
{
	bool newConnect = false;

	if( m_newConnections.size() > 0 )
		newConnect = true;

	return newConnect;
}

bool TcpServer::hasNewPackets()
{
	bool newPacket = false;

	if( m_newPackets.size() > 0 )
		newPacket = true;

	return newPacket;
}

void TcpServer::processMessages()
{
	while( getMessageCount() > 0 )
	{
		ProcessMessage* message = popMessage();

		if( message->type == MessageType::CLIENT_CONNECTED )
		{
			m_newConnections.push(
				static_cast< ProcessMessageClientConnected* >(message) );
		}
		else if( message->type == MessageType::RECEIVE_PACKET )
		{
			m_newPackets.push(
				static_cast< ProcessMessageReceivePacket* >(message) );
		}

		delete message;
	}
}
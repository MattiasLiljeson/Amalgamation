#include "TcpServer.h"

TcpServer::TcpServer()
{
	m_isListening = false;
	m_ioService = new boost::asio::io_service();
	m_listenerProcess = NULL;
}

TcpServer::~TcpServer()
{
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
	}
}

bool TcpServer::isListening()
{
	return m_isListening;
}

bool TcpServer::hasNewConnections()
{
	bool newConnect = false;

	while( getMessageCount() > 0 )
	{
		ProcessMessage* message = popMessage();

		if( message->type == MessageType::CLIENT_CONNECTED )
		{
			newConnect = true;
		}

		delete message;
	}

	return newConnect;
}
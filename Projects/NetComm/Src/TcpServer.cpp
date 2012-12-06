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

	for( unsigned int i=0; i<m_communicationProcesses.size(); i++ )
	{
		m_communicationProcesses[i]->putMessage( new ProcessMessageTerminate() );
		m_communicationProcesses[i]->stop();
		delete m_communicationProcesses[i];
	}

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

	if( m_newConnectionProcesses.size() > 0 )
		newConnect = true;

	return newConnect;
}

unsigned int TcpServer::newConnectionsCount()
{
	return m_newConnectionProcesses.size();
}

unsigned int TcpServer::activeConnectionsCount()
{
	return m_communicationProcesses.size();
}

unsigned int TcpServer::newDisconnectionsCount()
{
	return m_newDisconnectionProcesses.size();
}

bool TcpServer::hasNewPackets()
{
	bool newPacket = false;

	if( m_newPackets.size() > 0 )
		newPacket = true;

	return newPacket;
}

unsigned int TcpServer::newPacketsCount()
{
	return m_newPackets.size();
}

Packet* TcpServer::popNewPacket()
{
	Packet* packet = NULL;
	if ( !m_newPackets.empty() )
	{	
		packet = m_newPackets.front();
		m_newPackets.pop();
	}
	return packet;
}

void TcpServer::processMessages()
{
	while( getMessageCount() > 0 )
	{
		ProcessMessage* message = popMessage();

		if( message->type == MessageType::CLIENT_CONNECTED )
		{
			ProcessMessageClientConnected* messageClientConnected
				= static_cast< ProcessMessageClientConnected* >(message);

			m_communicationProcesses.push_back( new TcpCommunicationProcess(
				this, messageClientConnected->socket, m_ioService ) );
			m_communicationProcesses.back()->start();

			m_newConnectionProcesses.push( m_communicationProcesses.back()->getId() );
		}
		else if( message->type == MessageType::SOCKET_DISCONNECTED )
		{
			ProcessMessageSocketDisconnected* messageSocketDisconnected =
				static_cast< ProcessMessageSocketDisconnected* >(message);

			for( unsigned int i=0; i<m_communicationProcesses.size(); i++ )
			{
				if( messageSocketDisconnected->processId ==
					m_communicationProcesses[i]->getId() )
				{
					m_newDisconnectionProcesses.push(
						m_communicationProcesses[i]->getId() );
				}
			}
		}
		else if( message->type == MessageType::RECEIVE_PACKET )
		{
			m_newPackets.push(
				static_cast< ProcessMessageReceivePacket* >(message)->packet );
			cout << "TcpServer, receive: " <<
				m_newPackets.back()->getMessage() << endl;
		}

		delete message;
	}
}

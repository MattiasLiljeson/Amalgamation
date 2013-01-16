#include <boost/asio/io_service.hpp>
#include "TcpServer.h"

#include <exception>

#include "ProcessMessageClientConnected.h"
#include "ProcessMessageReceivePacket.h"
#include "ProcessMessageSendPacket.h"
#include "ProcessMessageSocketDisconnected.h"
#include "ProcessMessageTerminate.h"

#include "TcpCommunicationProcess.h"
#include "TcpListenerProcess.h"


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
		m_listenerProcess->putMessage( new ProcessMessageTerminate() );
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

bool TcpServer::hasNewDisconnections()
{
	bool newDisconnect = false;

	if( m_newDisconnectionProcesses.size() > 0 )
		newDisconnect = true;

	return newDisconnect;
}

int TcpServer::popNewDisconnection()
{
	int id = -1;
	if( m_newDisconnectionProcesses.size() > 0 )
	{
		id = m_newDisconnectionProcesses.front();
		m_newDisconnectionProcesses.pop();
	}

	return id;
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

Packet TcpServer::popNewPacket()
{
	
	if ( !m_newPackets.empty() )
	{	
		Packet packet = m_newPackets.front();
		m_newPackets.pop();
		return packet;
	}
	else
	{
		throw domain_error( "Trying to pop from an empty packet queue!" );
	}
	return NULL;
}

void TcpServer::processMessages()
{
	queue< ProcessMessage* > messages;
	messages = checkoutMessageQueue();

	while( messages.size() > 0 )
	{
		ProcessMessage* message = messages.front();
		messages.pop();

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

			int processToBeDeleted = -1;
			for( unsigned int i=0; i<m_communicationProcesses.size(); i++ )
			{
				if( messageSocketDisconnected->processId ==
					m_communicationProcesses[i]->getId() )
				{
					m_newDisconnectionProcesses.push(
						m_communicationProcesses[i]->getId() );

					processToBeDeleted = i;
					break;
				}
			}
			
			if (processToBeDeleted != -1)
			{
				m_communicationProcesses[processToBeDeleted]->putMessage( new ProcessMessageTerminate() );
				m_communicationProcesses[processToBeDeleted]->stop();
				delete m_communicationProcesses[processToBeDeleted];
				m_communicationProcesses.erase(m_communicationProcesses.begin() + processToBeDeleted);
			}
			else
				throw "Something is really knaaas";

		}
		else if( message->type == MessageType::RECEIVE_PACKET )
		{
			m_newPackets.push(
				static_cast< ProcessMessageReceivePacket* >(message)->packet );

		
		}

		delete message;
	}
}

void TcpServer::broadcastPacket( Packet p_packet )
{
	for( unsigned int i=0; i<m_communicationProcesses.size(); i++ )
	{
		m_communicationProcesses[i]->putMessage(
			new ProcessMessageSendPacket( this, p_packet ) );
	}
}

void TcpServer::multicastPacket( vector<int> p_connectionIdentities, Packet p_packet )
{
	for( unsigned int i=0; i<p_connectionIdentities.size(); i++ )
	{
		unicastPacket( p_packet, p_connectionIdentities[i] );
	}
}

void TcpServer::unicastPacket( Packet p_packet, int p_clientId )
{
	// NOTE: this might be slow enough to do for individual packets
	for ( unsigned int i = 0; i < m_communicationProcesses.size(); i++ )
	{
		if ( m_communicationProcesses[i]->getId() == p_clientId )
		{
			m_communicationProcesses[i]->putMessage(
				new ProcessMessageSendPacket( this, p_packet ) );

			break;
		}
	}
}

void TcpServer::unicastPacketQueue( queue<Packet> p_packets, int p_clientId )
{
	for ( unsigned int i = 0; i < m_communicationProcesses.size(); i++ )
	{
		if ( m_communicationProcesses[i]->getId() == p_clientId )
		{
			queue<ProcessMessage*> messages;
			while( !p_packets.empty() )
			{
				messages.push( new ProcessMessageSendPacket( this, p_packets.front() ) );
				p_packets.pop();
			}
			m_communicationProcesses[i]->putMessages( messages );

			break;
		}
	}
}

int TcpServer::popNewConnection()
{
	int id = -1;
	if( m_newConnectionProcesses.size() > 0 )
	{
		id = m_newConnectionProcesses.front();
		m_newConnectionProcesses.pop();
	}

	return id;
}

vector< int > TcpServer::getActiveConnections()
{
	vector< int > currentConnections;

	for( unsigned int i=0; i<m_communicationProcesses.size(); i++ )
	{
		currentConnections.push_back( m_communicationProcesses[i]->getId() );
	}

	return currentConnections;
}

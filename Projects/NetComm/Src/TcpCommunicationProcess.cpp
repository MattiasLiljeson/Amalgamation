#include "TcpCommunicationProcess.h"

#include <boost/thread/thread.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/asio/io_service.hpp>

#include "ProcessMessageSendPacket.h"
#include "ProcessMessageSocketDisconnected.h"
#include "ProcessMessageReceivePacket.h"

#include "ThreadSafeMessaging.h"
#include "ProcessMessagePacketOverflow.h"
#include <DebugUtil.h>
#include "ProcessMessageAskForCommProcessInfo.h"
#include "ProcessMessageCommProcessInfo.h"

TcpCommunicationProcess::TcpCommunicationProcess( ThreadSafeMessaging* p_parent, 
												 tcp::socket* p_socket,
												 boost::asio::io_service* p_ioService )
{
	m_running = false;

	m_ioService = p_ioService;

	m_parent = p_parent;

	m_activeSocket = p_socket;
	tcp::no_delay option( true );
	m_activeSocket->set_option( option );
	tcp::socket::non_blocking_io nonBlocking( false );	// If this setting is true it will
														// result in packet loss.
	m_activeSocket->io_control( nonBlocking );

	m_numberOfOverflowPackets = 0;
	m_totalPacketsReceived = 0;
	m_totalPacketsSent = 0;

	/************************************************************************/
	/* We need to find the appropriate size for received data buffer.		*/
	/************************************************************************/
	m_asyncDataCapacity = 512;
	m_asyncDataLength = 0;
	m_packetRestSize = 0;

	m_asyncData = new char[m_asyncDataCapacity];
	for(unsigned int i = 0; i < m_asyncDataCapacity; i++)
	{
		m_asyncData[i] = 0;
	}
}

TcpCommunicationProcess::~TcpCommunicationProcess()
{
	m_activeSocket->close();
	delete m_activeSocket;
	delete[] m_asyncData;
}

void TcpCommunicationProcess::body()
{
	m_running = true;

	startPacketReceiveCallback();

	while( m_running )
	{
		m_ioService->poll();

		processMessages();
		
		sleep(1);
	}
}

void TcpCommunicationProcess::processMessages()
{
	queue<ProcessMessage*> messages;
	messages = checkoutMessageQueue();

	while( messages.size() > 0 )
	{
//		sleep(1);
		ProcessMessage* message = messages.front();
		messages.pop();

		if( message->type == MessageType::TERMINATE )
		{
			m_running = false;
		}
		else if( message->type == MessageType::SEND_PACKET )
		{
			m_totalPacketsSent += 1;
			ProcessMessageSendPacket* sendPacketMessage =
				static_cast<ProcessMessageSendPacket*>(message);

			boost::system::error_code ec;
			m_activeSocket->send( boost::asio::buffer(
				sendPacketMessage->packet.getDataPtr(),
				sendPacketMessage->packet.getDataSize()),
				boost::asio::detail::message_do_not_route, ec );
			if( ec ) {
				DEBUGPRINT(( ec.message().c_str() ));
			}
		}
		else if( message->type == MessageType::ASK_FOR_COMM_PROCESS_INFO )
		{
			ProcessMessageAskForCommProcessInfo* askForInfoMessage =
				static_cast<ProcessMessageAskForCommProcessInfo*>(message);
			ThreadSafeMessaging* sender = askForInfoMessage->sender;
			ProcessMessageCommProcessInfo* respondInfo =
				new ProcessMessageCommProcessInfo( this, m_totalPacketsReceived, m_totalPacketsSent );
			sender->putMessage( respondInfo );
		}

		delete message;
	}
}

void TcpCommunicationProcess::startPacketReceiveCallback()
{
//	m_ioService->reset();
	m_activeSocket->async_receive(
		boost::asio::buffer( m_asyncData, m_asyncDataCapacity ),
		boost::bind( &TcpCommunicationProcess::onReceivePacket, this,
		boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred));
}

void TcpCommunicationProcess::onReceivePacket( const boost::system::error_code& p_error,
		size_t p_bytesTransferred )
{
	if( p_error == boost::asio::error::eof )
	{
		// Connection closed cleanly. // TEST THIS!!
		m_parent->putMessage( new ProcessMessageSocketDisconnected(
			this, getId() ) );

		m_running = false;
	}
	else if( p_error == boost::asio::error::connection_reset )
	{
		// (Connection aborted)
		
		m_parent->putMessage( new ProcessMessageSocketDisconnected(
			this, getId() ) );

		m_running = false;

	}
	else if( p_error == boost::asio::error::message_size )
	{
		DEBUGPRINT(( "Hey, you've got a message_size error dude!\n" ));
	}
	else if( p_error )
	{
		m_parent->putMessage( new ProcessMessageSocketDisconnected(
			this, getId() ) );

		m_running = false;
	}
	else
	{
		if( p_bytesTransferred > 0 )
		{
			queue< Packet > packets;

			unsigned int readPosition = 0;
			char* readPtr = m_asyncData;

			// Some debug info:
			vector<char> dbg_data;
			dbg_data.resize(p_bytesTransferred);
			memcpy(&dbg_data[0], m_asyncData, p_bytesTransferred);

			if( m_packetRestSize > 0 )
			{
				m_numberOfOverflowPackets += 1;
				m_parent->putMessage( new ProcessMessagePacketOverflow(
					m_numberOfOverflowPackets, this ) );

				unsigned int oldReserveBufferSize = m_reserveBuffer.size();
				m_reserveBuffer.resize( oldReserveBufferSize + m_packetRestSize );
				
				memcpy(&m_reserveBuffer[oldReserveBufferSize], readPtr, m_packetRestSize);

				// Create the whole packet now.
				Packet packet(getId(),&m_reserveBuffer[0], m_reserveBuffer.size());
				packets.push( packet );

				// Update readPosition to not start from 0.
				readPosition = m_packetRestSize;
				readPtr = m_asyncData + readPosition;

				// Clear the reserve buffer and its help variables
				m_reserveBuffer.clear();
				m_packetRestSize = 0;
			}

			while( readPosition < p_bytesTransferred )
			{
				unsigned int currentReadSize = (unsigned int)readPtr[0] + 1;
				
				if( readPosition + currentReadSize > p_bytesTransferred )
				{
					// PacketPartialSize is the size of the first part of the packet
					// which is split up between two buffers
					unsigned int packetFirstPartSize = p_bytesTransferred - readPosition;

					// and packetRestSize is the size of the second part of the packet.
					m_packetRestSize = currentReadSize - packetFirstPartSize;

					m_reserveBuffer.resize( packetFirstPartSize );
					memcpy(&m_reserveBuffer[0], readPtr, packetFirstPartSize);
					readPosition = p_bytesTransferred;
				}
				else
				{
					Packet packet(getId(), readPtr, currentReadSize );
					packets.push( packet );
					readPosition += currentReadSize;
					readPtr = m_asyncData + readPosition;
				}
			}
			m_totalPacketsReceived += packets.size();
			queue<ProcessMessage*> receivePacketMessages;
			while( !packets.empty() )
			{
				receivePacketMessages.push( new ProcessMessageReceivePacket(
					this,
					packets.front() ));
				packets.pop();
			}
			m_parent->putMessages( receivePacketMessages );

			startPacketReceiveCallback();
		}
	}
}
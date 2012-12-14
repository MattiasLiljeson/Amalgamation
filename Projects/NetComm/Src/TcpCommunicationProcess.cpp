#include "TcpCommunicationProcess.h"

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
	tcp::socket::non_blocking_io nonBlocking( true );
	m_activeSocket->io_control( nonBlocking );

	m_asyncDataLength = 0;
	m_asyncDataCapacity = 2048;
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
		boost::this_thread::sleep( boost::posix_time::millisec(1) );

		m_ioService->poll();

		processMessages();
		
	}
}

void TcpCommunicationProcess::processMessages()
{
	while( getMessageCount() > 0 )
	{
		ProcessMessage* message = popMessage();

		if( message->type == MessageType::TERMINATE )
		{
			m_running = false;
		}
		else if( message->type == MessageType::SEND_PACKET )
		{
			ProcessMessageSendPacket* sendPacketMessage =
				static_cast<ProcessMessageSendPacket*>(message);

			// HACK: Don't forget to actually HANDLE the error if it occurs :)
			boost::system::error_code ec;
			m_activeSocket->send( boost::asio::buffer(
				sendPacketMessage->packet.getDataPtr(),
				sendPacketMessage->packet.getDataSize()),
				boost::asio::detail::message_do_not_route,
				ec );

		}

		delete message;
	}
}

void TcpCommunicationProcess::startPacketReceiveCallback()
{
	m_ioService->reset();
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
			/// TODO: Fill packets queue with data using the messages queue.

			unsigned int readPosition = 0;
			char* readPtr = m_asyncData;

			while( readPosition < p_bytesTransferred )
			{
				unsigned int currentReadSize = (unsigned int)readPtr[0] + 1;
				Packet packet;
				packet.setData( readPtr, currentReadSize );
				packets.push( packet );
				readPosition += currentReadSize;
				readPtr = m_asyncData + readPosition;
			}



			while( !packets.empty() )
			{
				m_parent->putMessage( new ProcessMessageReceivePacket(
					this,
					packets.front() ) );
				packets.pop();
			}

			startPacketReceiveCallback();
		}
	}
}
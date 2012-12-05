#include "TcpCommunicationProcess.h"

TcpCommunicationProcess::TcpCommunicationProcess( ThreadSafeMessaging* p_parent, 
												 tcp::socket* p_socket )
{
	m_running = false;

	m_ioService = new boost::asio::io_service();

	m_parent = p_parent;

	m_activeSocket = p_socket;
	tcp::no_delay option( true );
	m_activeSocket->set_option( option );
	tcp::socket::non_blocking_io nonBlocking( true );
	m_activeSocket->io_control( nonBlocking );

	m_asyncDataLength = 0;
	m_asyncDataCapacity = 2048;
	m_asyncData = new char[m_asyncDataCapacity];
}

TcpCommunicationProcess::~TcpCommunicationProcess()
{
	m_activeSocket->close();
	delete m_activeSocket;
	delete[] m_asyncData;

	m_ioService->stop();
	delete m_ioService;
}

void TcpCommunicationProcess::body()
{
	m_running = true;

	startPacketReceiveCallback();

	while( m_running )
	{
		m_ioService->poll();

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

				m_activeSocket->send( boost::asio::buffer(
					sendPacketMessage->packet->getMessage() ) );
			}

			delete message;
		}
		
		boost::this_thread::sleep( boost::posix_time::millisec(1) );
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
		
	}
	else
	{
		if( p_bytesTransferred > 0 )
		{
			m_parent->putMessage( new ProcessMessageReceivePacket(
				this,
				new Packet(m_asyncData) ) );

			startPacketReceiveCallback();
		}
	}
}
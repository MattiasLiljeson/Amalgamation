#include "TcpCommunicationProcess.h"

TcpCommunicationProcess::TcpCommunicationProcess( tcp::socket* p_socket )
{
	m_activeSocket = p_socket;
	m_running = false;
}

TcpCommunicationProcess::~TcpCommunicationProcess()
{
}

void TcpCommunicationProcess::body()
{
	m_running = true;

	while( m_running )
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

				m_activeSocket->send( boost::asio::buffer(
					sendPacketMessage->packet->getMessage() ) );
			}

			delete message;
		}
		
		boost::this_thread::sleep( boost::posix_time::millisec(1) );
	}
}
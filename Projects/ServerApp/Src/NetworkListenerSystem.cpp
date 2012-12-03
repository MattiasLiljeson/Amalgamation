#include "NetworkListenerSystem.h"

NetworkListenerSystem::NetworkListenerSystem()
	: EntitySystem( SystemType::NetworkListenerSystem )
{
	m_ioService = NULL;
	m_tcpListenerProcess = NULL;
}

NetworkListenerSystem::~NetworkListenerSystem()
{
	if( m_ioService )
		delete m_ioService;	// Don't forget to call stop() before deletion!

	if( m_tcpListenerProcess )
	{
		m_tcpListenerProcess->putMessage( new ProcessMessageTerminate( this ) );
		m_tcpListenerProcess->stop(); // Blocks until process is done.
		delete m_tcpListenerProcess;
	}
}

void NetworkListenerSystem::process()
{
	queue< ProcessMessage* > messageQueue = checkoutMessageQueue(); /** Checkout thread-
																	safe messages */

	while( messageQueue.size() > 0 )
	{
		ProcessMessage* message = messageQueue.front();
		messageQueue.pop();

		// Handle message.
		switch( message->type )
		{
		case MessageType::CLIENT_CONNECTED:
			cout << "Someone connected!\n";
			break;
		default:
			break;
		}

		delete message;
	}

}

void NetworkListenerSystem::initialize()
{
	m_ioService = new boost::asio::io_service();

	m_tcpListenerProcess = new TcpListenerProcess( this, 1337, m_ioService );
	m_tcpListenerProcess->start();
}
#include "NetworkListenerSystem.h"

NetworkListenerSystem::NetworkListenerSystem()
	: EntitySystem( SystemType::NetworkListenerSystem )
{
	m_ioService = NULL;
	m_tcpListenerProcess = NULL;
}

NetworkListenerSystem::~NetworkListenerSystem()
{
	if( m_tcpListenerProcess )
	{
		m_tcpListenerProcess->putMessage( new ProcessMessageTerminate( this ) );

		m_tcpListenerProcess->stop(); // Blocks until process is done.
		delete m_tcpListenerProcess;
	}

	if( m_ioService )
	{
		m_ioService->stop();
		delete m_ioService;
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
		if( message->type == MessageType::CLIENT_CONNECTED )
		{
			cout << "Someone connected!\n";

			Entity* e = m_world->createEntity();
			e->addComponent( ComponentType::getTypeFor(ComponentType::Transform),
				new Transform( 10.0f, 0, 0 ) );
			e->addComponent( ComponentType::getTypeFor(ComponentType::NetworkSynced),
				new NetworkSynced() );
			m_world->addEntity( e );

			// TODO: Add an entity perhaps.

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
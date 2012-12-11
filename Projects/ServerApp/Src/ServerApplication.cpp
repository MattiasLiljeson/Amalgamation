#include "ServerApplication.h"

ServerApplication::ServerApplication()
{
	m_running = false;

	m_server = new TcpServer();
	m_server->startListening( 1337 );

	m_world = new EntityWorld();
	initSystems();
}

ServerApplication::~ServerApplication()
{
	delete m_world;
	delete m_server;
}

void ServerApplication::run()
{
	m_running = true;

	while( m_running )
	{
		// HACK: Static delta and really high for testing purposes.
		m_world->setDelta( 0.100f );

		m_world->process();

		// HACK: Maybe place input in systems? :D
		if( _kbhit() )
		{
			if( _getch() == 27 )
			{
				m_running = false;
			}
		}

		// HACK: Really slow update loop for testing purposes.
		boost::this_thread::sleep(boost::posix_time::milliseconds(100));
	}
}

void ServerApplication::initSystems()
{
	m_world->setSystem( SystemType::PrintPositionsSystem,
		new PrintPositionsSystem(), true );

	m_world->setSystem( SystemType::ProcessingMessagesSystem,
		new ProcessingMessagesSystem( static_cast< ThreadSafeMessaging* >(m_server) ),
		true );

	m_world->setSystem( SystemType::NetworkListenerSystem,
		new NetworkListenerSystem( m_server ), true );

	m_world->initialize();

}

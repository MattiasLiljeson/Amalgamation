#include "ServerApplication.h"

namespace Srv
{
	ServerApplication::ServerApplication()
	{
		m_running = false;

		m_server = new TcpServer();

		m_world = new EntityWorld();
		initSystems();
		initEntities();
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
			step( 0.01f );
		
			// HACK: Maybe place input in systems? :D
			if( _kbhit() )
			{
				if( _getch() == 27 )
				{
					m_running = false;
					_flushall();
				}
			}

			// HACK: Really slow update loop for testing purposes.
			boost::this_thread::sleep(boost::posix_time::milliseconds(10));
		}
	}

	void ServerApplication::step( float p_dt )
	{
		m_world->setDelta( p_dt );

		m_world->process();
	}

	void ServerApplication::initSystems()
	{
		/************************************************************************/
		/* Physics																*/
		/************************************************************************/
		PhysicsSystem* physics = new PhysicsSystem();
		m_world->setSystem(SystemType::PhysicsSystem, physics, true);

		m_world->setSystem( SystemType::PrintPositionsSystem,
			new PrintPositionsSystem(), true );

		m_world->setSystem( SystemType::ProcessingMessagesSystem,
			new ProcessingMessagesSystem( static_cast< ThreadSafeMessaging* >(m_server) ),
			true );

		m_world->setSystem( SystemType::NetworkListenerSystem,
			new NetworkListenerSystem( m_server ), true );

		m_world->setSystem( SystemType::NetworkInputHandlerSystem,
			new NetworkInputHandlerSystem( m_server ), true );

		m_world->setSystem( SystemType::NetworkUpdateSystem,
			new NetworkUpdateSystem( m_server ), true );

		m_world->initialize();

	}

	void ServerApplication::initEntities()
	{
	//	Entity* e;
	//
	//	e = m_world->createEntity();
	//	e->addComponent( ComponentType::getTypeFor( ComponentType::Transform ),
	//		new Transform( -10.0f, 0, 0 ) );
	//	e->addComponent( ComponentType::getTypeFor( ComponentType::NetworkSynced ),
	//		new NetworkSynced( e->getIndex(), -1, NetworkType::Ship ) );
	//	m_world->addEntity( e );
	//
	//	e = m_world->createEntity();
	//	e->addComponent( ComponentType::getTypeFor( ComponentType::Transform ),
	//		new Transform( 0, 0, 0 ) );
	//	e->addComponent( ComponentType::getTypeFor( ComponentType::NetworkSynced ),
	//		new NetworkSynced( e->getIndex(), -1, NetworkType::Ship ) );
	//	m_world->addEntity( e );
	}
};
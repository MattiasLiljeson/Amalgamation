#include "ServerApplication.h"

// Systems
#include <PhysicsSystem.h>
#include <ProcessingMessagesSystem.h>
#include <DebugPlayerScoresSystem.h>
#include <NetworkListenerSystem.h>
#include <NetworkInputHandlerSystem.h>
#include <NetworkUpdateSystem.h>
#include <NetworkUpdateScoresSystem.h>

#include "RenderInfo.h"
#include "Transform.h"
#include "PhysicsBody.h"
#include "BodyInitData.h"
#include "NetworkSynced.h"

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
//			boost::this_thread::sleep(boost::posix_time::milliseconds(10));
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

		m_world->setSystem( SystemType::ProcessingMessagesSystem,
			new ProcessingMessagesSystem( static_cast< ThreadSafeMessaging* >(m_server) ),
			true );

		m_world->setSystem( SystemType::NetworkListenerSystem,
			new NetworkListenerSystem( m_server ), true );

		m_world->setSystem( SystemType::NetworkInputHandlerSystem,
			new NetworkInputHandlerSystem( m_server ), true );

		m_world->setSystem( SystemType::NetworkUpdateSystem,
			new NetworkUpdateSystem( m_server ), true );

		m_world->setSystem( SystemType::NetworkUpdateScoresSystem,
			new NetworkUpdateScoresSystem( m_server ), true );

		m_world->setSystem( SystemType::DebugPlayerScoresSystem,
			new DebugPlayerScoresSystem(), true );

		m_world->initialize();

	}

	void ServerApplication::initEntities()
	{
		Entity* entity;
		Component* component;
		// Test physics
		// Entities on the server does not need any render info component

		//b1
		entity = m_world->createEntity();
		//component = new RenderInfo( cubeMeshId );
		//entity->addComponent( ComponentType::RenderInfo, component );
		component = new Transform(AglVector3(0, 0, 0), AglQuaternion(0, 0, 0, 1), AglVector3(1, 1, 1));
		entity->addComponent( ComponentType::Transform, component );
		component = new PhysicsBody();
		entity->addComponent(ComponentType::PhysicsBody, component);

		component = new BodyInitData(AglVector3(0, 0, 0), AglQuaternion::identity(),
			AglVector3(1, 1, 1), AglVector3(1, 0, 0), AglVector3(0, 0, 0), 0, false);
		entity->addComponent(ComponentType::BodyInitData, component);

		// The b1 entity should be synced over the network!
		component = new NetworkSynced(entity->getIndex(), -1, NetworkType::Prop);
		entity->addComponent(ComponentType::NetworkSynced, component);

		m_world->addEntity(entity);

		//b2
		entity = m_world->createEntity();
		//component = new RenderInfo( cubeMeshId );
		//entity->addComponent( ComponentType::RenderInfo, component );
		component = new Transform(AglVector3(15, 0.5f, 0.5f), AglQuaternion(0, 0, 0, 1), AglVector3(1, 1, 1));
		entity->addComponent( ComponentType::Transform, component );
		component = new PhysicsBody();
		entity->addComponent(ComponentType::PhysicsBody, component);

		component = new BodyInitData(AglVector3(15, 0.5f, 0.5f), AglQuaternion::identity(),
			AglVector3(1, 1, 1), AglVector3(-1, 0, 0), AglVector3(0, 0, 0), 0, true);
		entity->addComponent(ComponentType::BodyInitData, component);
		// Reminder: the b2 entity is static, hence, this entity could be left to exist
		// by default on the client, without any physics data. It doesn't need to be
		// synced over the network every frame, since it can't move. For now, it will be.
		component = new NetworkSynced(entity->getIndex(), -1, NetworkType::Prop);
		entity->addComponent(ComponentType::NetworkSynced, component);

		m_world->addEntity(entity);
	}
};
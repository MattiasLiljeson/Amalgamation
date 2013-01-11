#include "ServerApplication.h"

// Systems
#include <PhysicsSystem.h>
#include <ProcessingMessagesSystem.h>
#include <ServerWelcomeSystem.h>
#include <ServerPacketHandlerSystem.h>
#include <ServerUpdateSystem.h>
#include <ServerScoreSystem.h>
#include <ServerDynamicObjectsSystem.h>
#include <ServerStaticObjectsSystem.h>
#include <TimerSystem.h>

#include "RenderInfo.h"
#include "Transform.h"
#include "PhysicsBody.h"
#include "BodyInitData.h"
#include "NetworkSynced.h"
#include "StaticProp.h"

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

	void ServerApplication::body()
	{
		m_running = true;

		// simple timer
		__int64 countsPerSec = 0;
		__int64 currTimeStamp = 0;
		QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
		double secsPerCount = 1.0f / (float)countsPerSec;

		double dt = 0.0f;
		__int64 m_prevTimeStamp = 0;

		QueryPerformanceCounter((LARGE_INTEGER*)&m_prevTimeStamp);
		QueryPerformanceCounter((LARGE_INTEGER*)&currTimeStamp);

		while( m_running )
		{
			// Update timer
			QueryPerformanceCounter((LARGE_INTEGER*)&currTimeStamp);
			dt = (currTimeStamp - m_prevTimeStamp) * secsPerCount;

			m_prevTimeStamp = currTimeStamp;

			// HACK: Static delta and really high for testing purposes.
			step( static_cast<float>(dt) );

			// HACK: Maybe place input in systems? :D
			if( _kbhit() )
			{
				if( _getch() == 27 )
				{
					m_running = false;
					_flushall();
				}
			}
			processMessages();
			sleep(1);
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
		/* Timer																*/
		/************************************************************************/
		m_world->setSystem(SystemType::TimerSystem, new TimerSystem(), true);

		/************************************************************************/
		/* Physics																*/
		/************************************************************************/
		PhysicsSystem* physics = new PhysicsSystem();
		m_world->setSystem(SystemType::PhysicsSystem, physics, true);

		/************************************************************************/
		/* Objects Systems														*/
		/************************************************************************/
		m_world->setSystem(SystemType::ServerDynamicObjectsSystem, 
			new ServerDynamicObjectsSystem(), true);

		m_world->setSystem(SystemType::ServerStaticObjectsSystem, 
			new ServerStaticObjectsSystem(), true);

		/************************************************************************/
		/* Threading															*/
		/************************************************************************/
		m_world->setSystem( SystemType::ProcessingMessagesSystem,
			new ProcessingMessagesSystem( static_cast< ThreadSafeMessaging* >(m_server) ),
			true );

		/************************************************************************/
		/* Network																*/
		/************************************************************************/
		m_world->setSystem( SystemType::NetworkListenerSystem,
			new ServerWelcomeSystem( m_server ), true );

		m_world->setSystem( SystemType::ServerPacketHandlerSystem,
			new ServerPacketHandlerSystem( m_server ), true );

		m_world->setSystem( SystemType::NetworkUpdateSystem,
			new ServerUpdateSystem( m_server ), true );

		m_world->setSystem( SystemType::NetworkUpdateScoresSystem,
			new ServerScoreSystem( m_server ), true );

		m_world->initialize();

	}

	void ServerApplication::initEntities()
	{
		Entity* entity;
		Component* component;
		// Add a grid of cubes to test instancing.
		for( int x=0; x<8; x++ )
		{
			for( int y=0; y<8; y++ )
			{
				for( int z=0; z<8; z++ )
				{
					entity = m_world->createEntity();
					component = new Transform( 2.0f+5.0f*-x, 1.0f+5.0f*-y, 1.0f+5.0f*-z );
					entity->addComponent( ComponentType::Transform, component );
					entity->addComponent( ComponentType::StaticProp, new StaticProp());

					m_world->addEntity(entity);
				}
			}

		}
	}

	
	void ServerApplication::processMessages()
	{
		queue< ProcessMessage* > messages;
		messages = checkoutMessageQueue();

		while( messages.size() > 0 )
		{
			ProcessMessage* message = messages.front();
			messages.pop();

			if( message->type == MessageType::TERMINATE )
			{
				m_running = false;
			}
			delete message;
		}
	}

	void ServerApplication::run()
	{
		body();
	}

};
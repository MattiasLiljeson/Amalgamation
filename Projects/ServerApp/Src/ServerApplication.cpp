#include "ServerApplication.h"

#include "TcpServer.h"

#include <ComponentAssemblageAllocator.h>

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
#include <EntityFactory.h>
#include <LevelGenSystem.h>
#include <ServerPickingSystem.h>
#include <MinigunModuleControllerSystem.h>
#include <ShieldModuleControllerSystem.h>
#include <RocketLauncherModuleControllerSystem.h>
#include <MineControllerSystem.h>
#include <MineLayerModuleControllerSystem.h>
#include <ShipModulesControllerSystem.h>
#include <ShipManagerSystem.h>
#include <RocketControllerSystem.h>
#include <NetSyncedPlayerScoreTrackerSystem.h>
#include <ServerClientInfoSystem.h>
#include <LoadMeshSystemServer.h>
#include <LookAtSystem.h>

#include "Transform.h"
#include "PhysicsBody.h"
#include "BodyInitData.h"
#include "NetworkSynced.h"
#include "StaticProp.h"


//Modules
#include <ShipModule.h>
#include <MinigunModule.h>
#include <SpeedBoosterModule.h>
#include <RocketLauncherModule.h>
#include <ShieldModule.h>
#include <MineLayerModule.h>


namespace Srv
{
	ServerApplication::ServerApplication()
	{
		m_running = false;

		m_server = new TcpServer();

		m_world = new EntityWorld();
		initSystems();

		ComponentAssemblageAllocator allocator();

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
			// sleep(10);
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
		/* Entity creation														*/
		/************************************************************************/
		EntityFactory* factory = new EntityFactory(NULL, m_server);
		m_world->setSystem( factory, true);

		/************************************************************************/
		/* Timer																*/
		/************************************************************************/
		m_world->setSystem(SystemType::TimerSystem, new TimerSystem(), true);

		/************************************************************************/
		/* Mesh loading															*/
		/************************************************************************/
		// Note! Must set *after* EntityFactory and *before* Physics
		m_world->setSystem(SystemType::LoadMeshSystemServer, new LoadMeshSystemServer(), 
			true); 

		/************************************************************************/
		/* Level Generation														*/
		/************************************************************************/
		/*LevelGenSystem* levelGen = new LevelGenSystem( m_server);
		m_world->setSystem( levelGen, true);
		levelGen->run();*/

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
		/* General controlling													*/
		/************************************************************************/
		LookAtSystem* lookAtSystem = new LookAtSystem();
		m_world->setSystem(SystemType::LookAtSystem, lookAtSystem, true);

		/************************************************************************/
		/* Picking																*/
		/************************************************************************/
		m_world->setSystem(SystemType::ServerPickingSystem, new ServerPickingSystem(m_server), true);

		/************************************************************************/
		/* Gameplay															*/
		/************************************************************************/
		m_world->setSystem(new MinigunModuleControllerSystem(m_server), true);
		m_world->setSystem(new ShieldModuleControllerSystem(m_server), true);
		m_world->setSystem(new RocketLauncherModuleControllerSystem(m_server), true);
		m_world->setSystem(new MineLayerModuleControllerSystem(m_server), true);
		m_world->setSystem(new MineControllerSystem(m_server), true);
		m_world->setSystem(new ShipModulesControllerSystem(), true);
		m_world->setSystem(new ShipManagerSystem(), true);
		m_world->setSystem(new RocketControllerSystem(), true);


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
		m_world->setSystem( SystemType::NetSyncedPlayerScoreTrackerSystem,
			new NetSyncedPlayerScoreTrackerSystem(), true );

		m_world->setSystem( new ServerClientInfoSystem(), true);


		// NOTE: (Johan) THIS MUST BE AFTER ALL SYSTEMS ARE SET, OR SOME SYSTEMS WON'T
		// GET INITIALIZED. YES, I'M TALKING TO YOU ANTON :D
		m_world->initialize();




		// Run component assemblage allocator (not a system, so don't delete)
		ComponentAssemblageAllocator* allocator = new ComponentAssemblageAllocator();
		delete allocator;
	}

	void ServerApplication::initEntities()
	{
		InitModulesTestByAnton();
	}

	
	void ServerApplication::processMessages()
	{
		queue< ProcessMessage* > messages;
		messages = checkoutMessageQueue();

		while( messages.size() > 0 )
		{
			ProcessMessage* message = messages.front();
			messages.pop();

			if(message && message->type == MessageType::TERMINATE ){
				m_running = false;
			}
			delete message;
		}
	}

	void ServerApplication::run()
	{
		body();
	}
	void ServerApplication::InitModulesTestByAnton()
	{
		Entity* entity;
		Component* component;

		EntitySystem* tempSys = NULL;

		// Read from assemblage
		AssemblageHelper::E_FileStatus status = AssemblageHelper::FileStatus_OK;
		EntityFactory* factory = static_cast<EntityFactory*>
			( m_world->getSystem( SystemType::EntityFactory ) );


		// First test by Jarl, instead of Anton
		// Create rocks
		status = factory->readAssemblageFile( "Assemblages/rocksServer.asd" );
		entity = factory->entityFromRecipe( "rocksServer" );									 
		m_world->addEntity( entity );

		//Minigun
		for (int x=0;x<4;x++)
		{
			status = factory->readAssemblageFile( "Assemblages/minigunModule.asd" );
			entity = factory->entityFromRecipe( "MinigunModule" );
			//component = new Transform(10, 0, 0);
			//entity->addComponent( ComponentType::Transform, component );

			entity->addComponent( ComponentType::PhysicsBody, 
				new PhysicsBody() );

			entity->addComponent( ComponentType::BodyInitData, 
				new BodyInitData(AglVector3(10, 0, x*10),
				AglQuaternion::identity(),
				AglVector3(1, 1, 1), AglVector3(0, 0, 0), 
				AglVector3(0, 0, 0), 0, 
				BodyInitData::DYNAMIC, 
				BodyInitData::SINGLE, false));

			entity->addComponent(ComponentType::MinigunModule, new MinigunModule(AglVector3(0, 0, 0), AglVector3(0, 0, 1)));
			entity->addComponent(ComponentType::NetworkSynced, new NetworkSynced(entity->getIndex(), -1, EntityType::MinigunModule));
			m_world->addEntity(entity);
		}


		//Shield
		entity = m_world->createEntity();
		component = new Transform(20, 0, 0);
		entity->addComponent( ComponentType::Transform, component );

		entity->addComponent( ComponentType::PhysicsBody, 
			new PhysicsBody() );

		entity->addComponent( ComponentType::BodyInitData, 
			new BodyInitData(AglVector3(20, 0, 0),
			AglQuaternion::identity(),
			AglVector3(1, 1, 1), AglVector3(0, 0, 0), 
			AglVector3(0, 0, 0), 0, 
			BodyInitData::DYNAMIC, 
			BodyInitData::SINGLE, false));

		entity->addComponent(ComponentType::ShipModule, new ShipModule());
		entity->addComponent(ComponentType::ShieldModule, new ShieldModule());
		entity->addComponent(ComponentType::NetworkSynced, new NetworkSynced(entity->getIndex(), -1, EntityType::ShieldModule));
		m_world->addEntity(entity);

		for (int x=0;x<4;x++)
		{
			entity = m_world->createEntity();
			component = new Transform(30, 0, 0);
			((Transform*)component)->setScale(AglVector3(2,2,2));
			entity->addComponent( ComponentType::Transform, component );

			entity->addComponent( ComponentType::PhysicsBody, 
				new PhysicsBody() );

			entity->addComponent( ComponentType::BodyInitData, 
				new BodyInitData(AglVector3(30, 0, x*10),
				AglQuaternion::identity(),
				AglVector3(2, 2, 2), AglVector3(0, 0, 0), 
				AglVector3(0, 0, 0), 0, 
				BodyInitData::DYNAMIC, 
				BodyInitData::SINGLE, false));

			entity->addComponent(ComponentType::ShipModule, new ShipModule());
			entity->addComponent(ComponentType::RocketLauncherModule, new RocketLauncherModule(AglVector3(0, 0, 0), AglVector3(0, 0, 1)));
			entity->addComponent(ComponentType::NetworkSynced, new NetworkSynced(entity->getIndex(), -1, EntityType::RocketLauncherModule));
			m_world->addEntity(entity);
		}

		for (int x=0;x<4;x++)
		{
			entity = m_world->createEntity();
			component = new Transform(40, 0, 0);
			((Transform*)component)->setScale(AglVector3(2,2,2));
			entity->addComponent( ComponentType::Transform, component );

			entity->addComponent( ComponentType::PhysicsBody, 
				new PhysicsBody() );

			entity->addComponent( ComponentType::BodyInitData, 
				new BodyInitData(AglVector3(40, 0, x*10),
				AglQuaternion::identity(),
				AglVector3(2, 2, 2), AglVector3(0, 0, 0), 
				AglVector3(0, 0, 0), 0, 
				BodyInitData::DYNAMIC, 
				BodyInitData::SINGLE, false));

			entity->addComponent(ComponentType::MineLayerModule, new MineLayerModule());
			entity->addComponent(ComponentType::ShipModule, new ShipModule());
			entity->addComponent(ComponentType::NetworkSynced, new NetworkSynced(entity->getIndex(), -1, EntityType::MineLayerModule));
			m_world->addEntity(entity);
		}

		for (int x=0;x<4;x++)
		{
			entity = m_world->createEntity();
			component = new Transform(50, 0, 0);

			entity->addComponent( ComponentType::Transform, component );

			entity->addComponent( ComponentType::PhysicsBody, 
				new PhysicsBody() );

			entity->addComponent( ComponentType::BodyInitData, 
				new BodyInitData(AglVector3(50, 0, x*10),
				AglQuaternion::identity(),
				AglVector3(1, 1, 1), AglVector3(0, 0, 0), 
				AglVector3(0, 0, 0), 0, 
				BodyInitData::DYNAMIC, 
				BodyInitData::SINGLE, false));

			entity->addComponent(ComponentType::ShipModule, new ShipModule());
			entity->addComponent(ComponentType::SpeedBoosterModule, new SpeedBoosterModule());
			entity->addComponent(ComponentType::NetworkSynced, new NetworkSynced(entity->getIndex(), -1, EntityType::BoosterModule));
			m_world->addEntity(entity);
		}



		/*entity = m_world->createEntity();
		component = new Transform(50, 0, -10);
		entity->addComponent( ComponentType::Transform, component );

		entity->addComponent( ComponentType::PhysicsBody, 
			new PhysicsBody() );

		entity->addComponent( ComponentType::BodyInitData, 
			new BodyInitData(AglVector3(50, 0, -10),
			AglQuaternion::identity(),
			AglVector3(1, 1, 1), AglVector3(0, 0, 0), 
			AglVector3(0, 0, 0), 0, 
			BodyInitData::DYNAMIC, 
			BodyInitData::SINGLE, true, true));
		entity->addComponent(ComponentType::NetworkSynced, new NetworkSynced(entity->getIndex(), -1, EntityType::ShipModule));
		m_world->addEntity(entity);

		ConnectionPointSet* cpset = NULL;

		for(int i=0; i<1; i++)
		{
			entity = m_world->createEntity();
			component = new Transform(40, (float)i*10.0f, -10);
			entity->addComponent( ComponentType::Transform, component );

			entity->addComponent( ComponentType::PhysicsBody, 
				new PhysicsBody() );

			entity->addComponent( ComponentType::BodyInitData, 
				new BodyInitData(AglVector3(40, (float)i*10.0f, -10),
				AglQuaternion::identity(),
				AglVector3(1, 1, 1), AglVector3(0, 0, 0), 
				AglVector3(0, 0, 0), 0, 
				BodyInitData::DYNAMIC, 
				BodyInitData::SINGLE, false));

			entity->addComponent(ComponentType::ShipModule, new ShipModule());


			cpset = new ConnectionPointSet();
			AglMatrix target1 = AglMatrix::createTranslationMatrix(AglVector3(1, 2, 0));
			AglMatrix target2 = AglMatrix::createTranslationMatrix(AglVector3(-1, 2, 0));
			cpset->m_connectionPoints.push_back(ConnectionPoint(target1));
			cpset->m_connectionPoints.push_back(ConnectionPoint(target2));
			entity->addComponent(ComponentType::ConnectionPointSet, cpset);
			entity->addComponent(ComponentType::NetworkSynced, new NetworkSynced(entity->getIndex(), -1, EntityType::ShipModule));
			m_world->addEntity(entity);
		}*/


//		entity = m_world->createEntity();
//		component = new Transform(30, 0, -10);
//		entity->addComponent( ComponentType::Transform, component );
//
//		entity->addComponent( ComponentType::PhysicsBody, 
//			new PhysicsBody() );
//
//		entity->addComponent( ComponentType::BodyInitData, 
//			new BodyInitData(AglVector3(30, 0, -10),
//			AglQuaternion::identity(),
//			AglVector3(1, 1, 1), AglVector3(0, 0, 0), 
//			AglVector3(0, 0, 0), 0, 
//			BodyInitData::DYNAMIC, 
//			BodyInitData::SINGLE, false));
//
//		entity->addComponent(ComponentType::ShipModule, new ShipModule());
//
//
//		cpset = new ConnectionPointSet();
//		cpset->m_connectionPoints.push_back(ConnectionPoint(target1));
//		cpset->m_connectionPoints.push_back(ConnectionPoint(target2));
//		entity->addComponent(ComponentType::ConnectionPointSet, cpset);
//		entity->addComponent(ComponentType::NetworkSynced, new NetworkSynced(entity->getIndex(), -1, EntityType::ShipModule));
//		m_world->addEntity(entity); 


		//Ray entity
		/*entity = m_world->createEntity();
		component = new RenderInfo( cubeMeshId );
		entity->addComponent( ComponentType::RenderInfo, component );


		Transform* t = new Transform(AglVector3(0, 0, 0), AglQuaternion::rotateToFrom(AglVector3(0, 0, 1), AglVector3(0, 1, 0)), AglVector3(0.1f, 0.1f, 10));
		entity->addComponent( ComponentType::Transform, t);
		m_world->addEntity(entity);*/
	}


};
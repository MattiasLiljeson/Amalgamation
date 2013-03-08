#include "ServerApplication.h"

#include "TcpServer.h"
#include <ComponentAssemblageAllocator.h>
// Systems
#include "BodyInitData.h"
#include "NetworkSynced.h"
#include "PhysicsBody.h"
#include "StaticProp.h"
#include "Transform.h"
#include <AnomalyBombControllerSystem.h>
#include <EntityFactory.h>
#include <LevelGenSystem.h>
#include <LevelHandlerSystem.h>
#include <LoadMeshSystemServer.h>
#include <LookAtSystem.h>
#include <MineControllerSystem.h>
#include <MineLayerModuleControllerSystem.h>
#include <MinigunModuleControllerSystem.h>
#include <ModuleVisualEffectServerBufferSystem.h>
#include <NetSyncedPlayerScoreTrackerSystem.h>
#include <OutputLogger.h>
#include <PhysicsSystem.h>
#include <ProcessingMessagesSystem.h>
#include <RocketControllerSystem.h>
#include <RocketLauncherModuleControllerSystem.h>
#include <ServerClientInfoSystem.h>
#include <ServerDynamicObjectsSystem.h>
#include <ServerDynamicPhysicalObjectsSystem.h>
#include <ServerPacketHandlerSystem.h>
#include <ServerPickingSystem.h>
#include <ServerScoreSystem.h>
#include <ServerStaticObjectsSystem.h>
#include <ServerUpdateSystem.h>
#include <ServerWelcomeSystem.h>
#include <ShieldModuleControllerSystem.h>
#include <ShipManagerSystem.h>
#include <ShipModulesControllerSystem.h>
#include <ShipModulesTrackerSystem.h>
#include <SpawnPointSystem.h>
#include <TempModuleSpawner.h>
#include <TeslaCoilModuleControllerSystem.h>
#include <TimerSystem.h>
#include <WinningConditionSystem.h>

//Modules
#include <MineLayerModule.h>
#include <MinigunModule.h>
#include <ParticleSystemServerComponent.h>
#include <RocketLauncherModule.h>
#include <ServerMeasurementSystem.h>
#include <ShieldModule.h>
#include <ShipModule.h>
#include <SpeedBoostModuleControllerSystem.h>
#include <SpeedBoosterModule.h>
#include <SpawnPointSet.h>
#include <EnumGameStates.h>
#include <ServerGameState.h>
#include <ServerStateSystem.h>
#include <AnomalyAcceleratorModuleControllerSystem.h>
#include <ShipModuleStatsSystem.h>
#include <PlayerSystem.h>


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
		float dtFactor = 1.0f;
		__int64 m_prevTimeStamp = 0;

		QueryPerformanceCounter((LARGE_INTEGER*)&m_prevTimeStamp);
		QueryPerformanceCounter((LARGE_INTEGER*)&currTimeStamp);

		while( m_running )
		{
			// Update timer
			QueryPerformanceCounter((LARGE_INTEGER*)&currTimeStamp);
			dt = (currTimeStamp - m_prevTimeStamp) * secsPerCount;

			m_prevTimeStamp = currTimeStamp;

			step( static_cast<float>(dt * dtFactor) );

			if( _kbhit() )
			{
				int key = _getch();
				if( key == 27 )
				{
					m_running = false;
					_flushall();
				}
				else if( key == 'i' )
				{
					dtFactor += 0.1f;
				}
				else if( key == 'o' )
				{
					dtFactor -= 0.1f;
				}
				cout << dtFactor << endl;
			}
			processMessages();
			
			sleep(2);
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
		/* States																*/
		/************************************************************************/
		ServerStateSystem* serverStates = new ServerStateSystem(ServerStates::LOBBY);
		m_world->setSystem(serverStates,true);

		/************************************************************************/
		/* Entity creation														*/
		/************************************************************************/
		EntityFactory* factory = new EntityFactory(NULL, m_server);
		m_world->setSystem( factory, true);

		/************************************************************************/
		/* Timer																*/
		/************************************************************************/
		m_world->setSystem(new TimerSystem(), true);

		/************************************************************************/
		/* Level Handler														*/
		/************************************************************************/
		LevelHandlerSystem* levelHandler = new LevelHandlerSystem();
		m_world->setSystem( levelHandler, true);

		/************************************************************************/
		/* Mesh loading															*/
		/************************************************************************/
		// Note! Must set *after* EntityFactory and *before* Physics
		m_world->setSystem(new LoadMeshSystemServer(), true); 

		/************************************************************************/
		/* Level Generation														*/
		/************************************************************************/
		LevelGenSystem* levelGen = new LevelGenSystem( m_server);
		m_world->setSystem( levelGen, true);

		/************************************************************************/
		/* Effects																*/
		/************************************************************************/
		auto moduleeffect = new ModuleVisualEffectServerBufferSystem(m_server,serverStates);
		m_world->setSystem(moduleeffect, true);
		m_world->setSystem(new ShipModuleStatsSystem(moduleeffect), true);

		/************************************************************************/
		/* Physics																*/
		/************************************************************************/
		PhysicsSystem* physics = new PhysicsSystem(m_server);
		m_world->setSystem(physics, true);

		/************************************************************************/
		/* Objects Systems														*/
		/************************************************************************/
		m_world->setSystem(new ServerDynamicObjectsSystem(), true);
		m_world->setSystem(new ServerStaticObjectsSystem(), true);

		/************************************************************************/
		/* Threading															*/
		/************************************************************************/
		m_world->setSystem(new ProcessingMessagesSystem(
			static_cast< ThreadSafeMessaging* >(m_server) ), true );

		/************************************************************************/
		/* General controlling													*/
		/************************************************************************/
		LookAtSystem* lookAtSystem = new LookAtSystem(m_server);
		m_world->setSystem(lookAtSystem, true);


		/************************************************************************/
		/* Picking																*/
		/************************************************************************/
		m_world->setSystem(new ServerPickingSystem(m_server,moduleeffect), true);


		//Närverk var här innan


		/************************************************************************/
		/* Game play															*/
		/************************************************************************/
		m_world->setSystem(new ServerDynamicPhysicalObjectsSystem(), true);
		m_world->setSystem(new MinigunModuleControllerSystem(m_server), true);
		m_world->setSystem(new RocketLauncherModuleControllerSystem(m_server), true);
		m_world->setSystem(new MineLayerModuleControllerSystem(m_server), true);
		m_world->setSystem(new MineControllerSystem(m_server), true);
		m_world->setSystem(new AnomalyAcceleratorModuleControllerSystem(m_server), true);
		m_world->setSystem(new AnomalyBombControllerSystem(m_server), true);
		m_world->setSystem(new TeslaCoilModuleControllerSystem(m_server));
		m_world->setSystem(new ShipManagerSystem(), true);
		m_world->setSystem(new RocketControllerSystem(m_server), true);
		m_world->setSystem(new SpeedBoostModuleControllerSystem(m_server), true);
		m_world->setSystem(new ShieldModuleControllerSystem(m_server), true);
		// Important for any module-damaging logic to happen before this.
		m_world->setSystem(new ShipModulesControllerSystem(m_server,moduleeffect), true);
		m_world->setSystem(new ShipModulesTrackerSystem(), true);

		m_world->setSystem(new PlayerSystem(), true);

		//WinningConditionSystem* winningCondition = new WinningConditionSystem(m_server);
		m_world->setSystem(new WinningConditionSystem(m_server), true);
		m_world->setSystem(new SpawnPointSystem(), true);
		m_world->setSystem(new TempModuleSpawner(m_server), true);

		/************************************************************************/
		/* Network																*/
		/************************************************************************/
		m_world->setSystem(new ServerWelcomeSystem( m_server ), true);
		m_world->setSystem(new ServerPacketHandlerSystem( m_server ), true);
		m_world->setSystem(new ServerUpdateSystem( m_server ), true);
		m_world->setSystem(new ServerScoreSystem( m_server ), true);
		m_world->setSystem(new NetSyncedPlayerScoreTrackerSystem(), true);
		m_world->setSystem(new ServerClientInfoSystem(), true);

		// NOTE: (Johan) Should be called from some lobby-to-in-game state change:
//		winningCondition->startGameSession(20.0f);

		/************************************************************************/
		/* Debugging															*/
		/************************************************************************/
		m_world->setSystem(new ServerMeasurementSystem(), true);
		m_world->setSystem(new OutputLogger("log_server.txt"));

		// NOTE: (Johan) THIS MUST BE AFTER ALL SYSTEMS ARE SET, OR SOME SYSTEMS WON'T
		// GET INITIALIZED.
		m_world->initialize();

		// Run component assemblage allocator (not a system, so don't delete)
		ComponentAssemblageAllocator* allocator = new ComponentAssemblageAllocator();
		delete allocator; // NOTE: (Johan) Why u keep deleting it then?

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


		// First test by Jarl, instead of Anton.
		// Commented out by Alex, since the assemblage is now read in the LevelGenSystem.
		// Create rocks
		//status = factory->readAssemblageFile( "Assemblages/rocksServer.asd" );
		//entity = factory->entityFromRecipe( "rocksServer" );									 
		//m_world->addEntity( entity );

		EntityCreationPacket cp;
		cp.scale = AglVector3(1.0f, 1.0f, 1.0f);
/*		//Rocket Launcher
		for (unsigned int i = 0; i < 4; i++)
		{
			AglMatrix pos = AglMatrix::createTranslationMatrix(AglVector3(40.0f, 0.0f, (float)i*15.0f));
			cp.entityType = EntityType::RocketLauncherModule;
			factory->entityFromPacket(cp, &pos);
		}
		//Mine Layer
		for (unsigned int i = 0; i < 4; i++)
		{
			AglMatrix pos = AglMatrix::createTranslationMatrix(AglVector3(30.0f, 0.0f, (float)i*15.0f));
			cp.entityType = EntityType::MineLayerModule;
			factory->entityFromPacket(cp, &pos);
		}
		//Shield modules
		for (unsigned int i = 0; i < 4; i++)
		{
			AglMatrix pos = AglMatrix::createTranslationMatrix(AglVector3(50.0f, 0.0f, (float)i*15.0f));
			cp.entityType = EntityType::ShieldModule;
			factory->entityFromPacket(cp, &pos);
		}

		//Speed Booster
		for (unsigned int i = 0; i < 4; i++)
		{
			AglMatrix pos = AglMatrix::createTranslationMatrix(AglVector3(60.0f, 0.0f, (float)i*15.0f));
			cp.entityType = EntityType::BoosterModule;
			factory->entityFromPacket(cp, &pos);
		}
		//Minigun
		for (unsigned int i = 0; i < 4; i++)
		{
			AglMatrix pos = AglMatrix::createTranslationMatrix(AglVector3(70.0f, 0.0f, (float)i*15.0f));
			cp.entityType = EntityType::MinigunModule;
			factory->entityFromPacket(cp, &pos);
		}
		//Anomaly accelerator
		for (unsigned int i = 0; i < 4; i++)
		{
			AglMatrix pos = AglMatrix::createTranslationMatrix(AglVector3(80.0f, 0.0f, (float)i*15.0f));
			cp.entityType = EntityType::AnomalyModule;
			factory->entityFromPacket(cp, &pos);
		}
		//Tesla coil
		for (unsigned int i = 0; i < 4; i++)
		{
			AglMatrix pos = AglMatrix::createTranslationMatrix(AglVector3(90.0f, 0.0f, (float)i*15.0f));
			cp.entityType = EntityType::TeslaCoilModule;
			factory->entityFromPacket(cp, &pos);
		}*/
	}
};
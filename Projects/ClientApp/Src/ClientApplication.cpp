#include "ClientApplication.h"
#include <windows.h>

#ifdef COMBINE_CLIENT_AND_SERVER
	#include "ServerApplication.h"
#endif

#include <EntityWorld.h>
#include <Input.h>
#include <ComponentAssemblageAllocator.h>

// Components
#include <AudioInfo.h>
#include <AudioListener.h>
#include <BodyInitData.h>
#include <CircularMovement.h>
#include <ConnectionPointSet.h>
#include <Connector1to2Module.h>
#include <DebugMove.h>
#include <EntityParent.h>
#include <GameplayTags.h>
#include <HudElement.h>
#include <InterpolationComponent.h>
#include <LightsComponent.h>
#include <LoadMesh.h>
#include <MineLayerModule.h>
#include <MinigunModule.h>
#include <MinigunModule.h>
#include <ParticleSystemsComponent.h>
#include <PhysicsBody.h>
#include <PlayerCameraController.h>
#include <PositionalSoundSource.h>
#include <RenderInfo.h>
#include <RocketLauncherModule.h>
#include <ShieldModule.h>
#include <ShipEditController.h>
#include <ShipFlyController.h>
#include <ShipModule.h>
#include <SpeedBoosterModule.h>
#include <Transform.h>
#include <GameState.h>

// Systems
#include <AntTweakBarEnablerSystem.h>
#include <AntTweakBarSystem.h>
#include <AudioBackendSystem.h>
#include <AudioController.h>
#include <AudioListenerSystem.h>
#include <AxisRotationSystem.h>
#include <CameraInfo.h>
#include <CameraSystem.h>
#include <CircularMovementSystem.h>
#include <ClientConnectToServerSystem.h>
#include <ClientEntityCountSystem.h>
#include <ClientMeasurementSystem.h>
#include <ClientPacketHandlerSystem.h>
#include <ClientPickingSystem.h>
#include <DebugMovementSystem.h>
#include <DisplayPlayerScoreSystem.h>
#include <EntityFactory.h>
#include <ExtrapolationSystem.h>
#include <GameOptionsSystem.h>
#include <GameStatsSystem.h>
#include <GamepadRumbleSystem.h>
#include <GraphicsBackendSystem.h>
#include <GraphicsRendererSystem.h>
#include <HudSystem.h>
#include <InputActionsBackendSystem.h>
#include <InputBackendSystem.h>
#include <InterpolationSystem.h>
#include <LevelGenSystem.h>
#include <LibRocketBackendSystem.h>
#include <LibRocketEventManagerSystem.h>
#include <LightBlinkerSystem.h>
#include <LightRenderSystem.h>
#include <LoadMeshSystemClient.h>
#include <LookAtEntity.h>
#include <LookAtSystem.h>
#include <MenuSystem.h>
#include <MeshRenderSystem.h>
#include <MineControllerSystem.h>
#include <MineLayerModuleControllerSystem.h>
#include <MinigunModuleControllerSystem.h>
#include <MoveShipLightsSystem.h>
#include <NetSyncedPlayerScoreTrackerSystem.h>
#include <NetsyncDirectMapperSystem.h>
#include <ParticleRenderSystem.h>
#include <PhysicsSystem.h>
#include <PlayerCameraControllerSystem.h>
#include <PositionalSoundSystem.h>
#include <ProcessingMessagesSystem.h>
#include <RocketLauncherModuleControllerSystem.h>
#include <ShadowSystem.h>
#include <ShieldModuleControllerSystem.h>
#include <ShieldPlatingSystem.h>
#include <ShipEditControllerSystem.h>
#include <ShipFlyControllerSystem.h>
#include <ShipInputProcessingSystem.h>
#include <ShipModulesControllerSystem.h>
#include <TimerSystem.h>
#include <TransformParentHandlerSystem.h>
#include <ScoreWorldVisualizerSystem.h>
#include <ParticleSystemInstructionTranslatorSystem.h>
#include <ClientEntityCountSystem.h>
#include <SkeletalAnimationSystem.h>
#include <LevelHandlerSystem.h>
#include <CullingSystem.h>
#include <ConnectionVisualizerSystem.h>
#include <SpeedFovAdjustSystem.h>
#include <MenuBackgroundSceneSystem.h>
#include <LevelInfoLoader.h>
#include <EnvironmentSystem.h>
#include <SpeedBufferUpdaterSystem.h>
#include <ShipParticleSystemUpdater.h>
#include <EditSphereSystem.h>
#include <SelectionMarkerSystem.h>

// Helpers
#include <ConnectionPointCollection.h>
#include <vector>

using namespace std;

// MISC
#include <time.h>
#include <AntTweakBarWrapper.h>
#include <LightInstanceData.h>
#include <ShipSlotControllerSystem.h>
#include <MeshOffsetTransform.h>
#include <RandomUtil.h>
#include <DestroyOnParticlesDeathSystem.h>

#define FORCE_VS_DBG_OUTPUT


ClientApplication::ClientApplication( HINSTANCE p_hInstance )
{
		m_running = false;
		m_hInstance = p_hInstance;
		m_client = new TcpClient();
		m_world = new EntityWorld();

#ifdef COMBINE_CLIENT_AND_SERVER
		m_serverApp = new Srv::ServerApplication();
#endif
		// Systems first!
		initSystems();

		// Test entities later!
		initEntities();
}

ClientApplication::~ClientApplication()
{

#ifdef COMBINE_CLIENT_AND_SERVER
	ProcessMessage* newMessage = new ProcessMessage(MessageType::TERMINATE,NULL);
	m_serverApp->putMessage( newMessage );
	m_serverApp->stop();
	delete m_serverApp;
#endif
	delete m_world;
	delete m_client;
}

void ClientApplication::run()
{
#ifdef COMBINE_CLIENT_AND_SERVER
	m_serverApp->start();
#endif
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

	MSG msg = {0};
	while(m_running)
	{
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			QueryPerformanceCounter((LARGE_INTEGER*)&currTimeStamp);
			dt = (currTimeStamp - m_prevTimeStamp) * secsPerCount;

			m_prevTimeStamp = currTimeStamp;

			if(dt > 0.5f)
				dt = 0.5f;

			m_world->setDelta((float)dt);
			m_world->process();

			if(m_world->shouldShutDown())
				m_running = false;
			
		}

	}
}

void ClientApplication::initSystems()
{
	//----------------------------------------------------------------------------------
	// Systems must be added in the order they are meant to be executed. The order the
	// systems are added here is the order the systems will be processed
	//----------------------------------------------------------------------------------

	/************************************************************************/
	/* TimerSystem used by other systems should be first.					*/
	/************************************************************************/
	m_world->setSystem(SystemType::TimerSystem, new TimerSystem(), true);

	/************************************************************************/
	/* Graphics																*/
	/************************************************************************/
	GraphicsBackendSystem* graphicsBackend = new GraphicsBackendSystem( m_hInstance ,
		1280,720,true);

	m_world->setSystem( graphicsBackend, true );

	/************************************************************************/
	/* Entity creation														*/
	/************************************************************************/
	EntityFactory* factory = new EntityFactory(m_client, NULL);
	m_world->setSystem( factory, true);

	/************************************************************************/
	/* Level handling														*/
	/************************************************************************/
	LevelHandlerSystem* levelHandler = new LevelHandlerSystem();
	m_world->setSystem( levelHandler, true);
	m_world->setSystem( new LevelInfoLoader(), true );

	/************************************************************************/
	/* Mesh loading															*/
	/************************************************************************/
	// Note! Must set *after* EntityFactory and GraphicsBackend, and *before* Physics
	m_world->setSystem(SystemType::LoadMeshSystem, new LoadMeshSystemClient(graphicsBackend), 
						true); 
	m_world->setSystem(new ParticleSystemInstructionTranslatorSystem( graphicsBackend ),
		true );
	/************************************************************************/
	/* Physics																*/
	/************************************************************************/
	PhysicsSystem* physics = new PhysicsSystem(NULL);
	m_world->setSystem(SystemType::PhysicsSystem, physics, true);

	/************************************************************************/
	/* General controlling													*/
	/************************************************************************/
	m_world->setSystem( new LookAtSystem() );
	m_world->setSystem( new SpeedBufferUpdaterSystem() );
	
	/************************************************************************/
	/* Input																*/
	/************************************************************************/
	InputBackendSystem* inputBackend = new InputBackendSystem( m_hInstance, 
		graphicsBackend );
	m_world->setSystem( inputBackend, true);
	m_world->setSystem( new InputActionsBackendSystem( SETTINGSPATH + "input.ini" ),
		true );

	GamepadRumbleSystem* gamepadRumble = new GamepadRumbleSystem( inputBackend );
	m_world->setSystem( gamepadRumble, true);

	/************************************************************************/
	/* Culling																*/
	/************************************************************************/
	m_world->setSystem(new CullingSystem(), true);

	
	/************************************************************************/
	/* GUI																	*/
	/************************************************************************/
	LibRocketBackendSystem* rocketBackend = new LibRocketBackendSystem( graphicsBackend,
		inputBackend );
	m_world->setSystem( rocketBackend, true );

	HudSystem* hud = new HudSystem( rocketBackend );
	m_world->setSystem( hud, true );

	m_world->setSystem( new LibRocketEventManagerSystem(), true );
	m_world->setSystem( new GameOptionsSystem() );

	// NOTE: MenuSystem looks up all systems that's also deriving from EventHandler, so
	// that they can be properly be added to the LibRocketEventManager.
	// The alternative would be that every event handler adds itself.
	m_world->setSystem( new MenuSystem(), true );


	/************************************************************************/
	/* Effects																*/
	/************************************************************************/
	m_world->setSystem( new ScoreWorldVisualizerSystem() );
	m_world->setSystem( new ConnectionVisualizerSystem() );
	m_world->setSystem( new ShipParticleSystemUpdater() );
	m_world->setSystem( new EditSphereSystem() );
	m_world->setSystem( new SelectionMarkerSystem());

	/************************************************************************/
	/* Player    															*/
	/************************************************************************/
	// Input system for ships
	ShipInputProcessingSystem* shipInputProc = new ShipInputProcessingSystem(inputBackend);
	m_world->setSystem( shipInputProc, true);
	SlotInputControllerSystem* slotInput = new SlotInputControllerSystem(inputBackend, m_client);
	m_world->setSystem( slotInput, true );

	// Controller systems for the ship
	ShipFlyControllerSystem* shipFlyController = new ShipFlyControllerSystem(shipInputProc, physics,
		m_client, slotInput );
	m_world->setSystem( shipFlyController, true);

	ShipEditControllerSystem* shipEditController = new ShipEditControllerSystem(shipInputProc, physics,
		slotInput);
	m_world->setSystem( shipEditController, true);


	/************************************************************************/
	/* Hierarchy															*/
	/************************************************************************/
	EntityParentHandlerSystem* entityParentHandler = new EntityParentHandlerSystem();
	m_world->setSystem( entityParentHandler, true );


	/************************************************************************/
	/* Camera																*/
	/************************************************************************/
	// Chamber fog and ambient
	m_world->setSystem( new EnvironmentSystem() );

	// Controller logic for camera
	m_world->setSystem( new PlayerCameraControllerSystem( shipInputProc, m_client ) );
	// Camera system sets its viewport info to the graphics backend for render
	m_world->setSystem( new CameraSystem( graphicsBackend ) );

	m_world->setSystem( new SpeedFovAdjustSystem(), true );


	/************************************************************************/
	/* Renderer																*/
	/************************************************************************/
	MeshRenderSystem* renderer = new MeshRenderSystem( graphicsBackend );
	m_world->setSystem( renderer , true );

	ParticleRenderSystem* particleRender = new ParticleRenderSystem( graphicsBackend );
	m_world->setSystem( particleRender, true );

	LightRenderSystem* lightRender = new LightRenderSystem( graphicsBackend );
	m_world->setSystem( lightRender, true );
	
	AntTweakBarSystem* antTweakBar = new AntTweakBarSystem( graphicsBackend, inputBackend );
	m_world->setSystem( antTweakBar, false );

	ShadowSystem* shadowSystem = new ShadowSystem ();
	m_world->setSystem( shadowSystem, true );

	/************************************************************************/
	/* Network																*/
	/************************************************************************/
	ProcessingMessagesSystem* msgProcSystem = new ProcessingMessagesSystem( m_client );
	m_world->setSystem( msgProcSystem , true );

	ClientConnectToServerSystem* connect =
		new ClientConnectToServerSystem( m_client, false);
	m_world->setSystem( connect, true );
	m_world->setSystem( new NetsyncDirectMapperSystem(), true );
	m_world->setSystem( new NetSyncedPlayerScoreTrackerSystem(), true );
	ClientPacketHandlerSystem* communicatorSystem =
		new ClientPacketHandlerSystem( m_client );
	m_world->setSystem( communicatorSystem, false );
	m_world->setSystem( new ExtrapolationSystem(m_client), true );

	/************************************************************************/
	/* Interpolation  														*/
	/************************************************************************/
	// InterpolationSystem* interpolationSystem = new InterpolationSystem();
	// m_world->setSystem( interpolationSystem, true);


	/************************************************************************/
	/* Audio																*/
	/************************************************************************/
#ifdef ENABLE_SOUND
	AudioBackendSystem* audioBackend = new AudioBackendSystem();
	m_world->setSystem( audioBackend, true );
	AudioController* audioController = new AudioController(audioBackend);
	m_world->setSystem( audioController, true );
	AudioListenerSystem* audioListener = new AudioListenerSystem(audioBackend);
	m_world->setSystem( audioListener, true );
	m_world->setSystem( new PositionalSoundSystem(), true );
#endif // ENABLE_SOUND

	/************************************************************************/
	/* Gameplay																*/
	/************************************************************************/
	m_world->setSystem( new DisplayPlayerScoreSystem(m_client), true );
	m_world->setSystem( new ClientPickingSystem(m_client), true );
	m_world->setSystem( new GameStatsSystem(), true );
	m_world->setSystem( new LightBlinkerSystem(), true );
	m_world->setSystem( new ShieldPlatingSystem(), true );

	/************************************************************************/
	/* Animation															*/
	/************************************************************************/
	m_world->setSystem( new SkeletalAnimationSystem(), true );

	/************************************************************************/
	/* Graphics representer													*/
	/************************************************************************/
	GraphicsRendererSystem* graphicsRender = new GraphicsRendererSystem(graphicsBackend,
		shadowSystem, renderer, rocketBackend, particleRender, antTweakBar, lightRender);
	m_world->setSystem( graphicsRender, true );

	/************************************************************************/
	/* Destroyers															*/
	/************************************************************************/
	m_world->setSystem( new DestroyOnParticlesDeathSystem(), true );

	/************************************************************************/
	/* Debugging															*/
	/************************************************************************/
	m_world->setSystem( new DebugMovementSystem(), false );
	m_world->setSystem( new MenuBackgroundSceneSystem(), true );
	m_world->setSystem( new CircularMovementSystem(), true );
	m_world->setSystem( new AxisRotationSystem(), true );
	m_world->setSystem( new MoveShipLightsSystem(), true );
	m_world->setSystem( new ClientMeasurementSystem(), true );
	m_world->setSystem( new ClientEntityCountSystem(), true );
	m_world->setSystem( new AntTweakBarEnablerSystem(), true );

	m_world->initialize();

	// Run component assemblage allocator (not a system, so don't delete)
	ComponentAssemblageAllocator* allocator = new ComponentAssemblageAllocator();
	delete allocator; // NOTE: (Johan) Why u delete when u say "don't delete"?
}

void ClientApplication::initEntities()
{
	Entity* entity = NULL;
	Component* component = NULL;

	// Read from assemblage
	AssemblageHelper::E_FileStatus status = AssemblageHelper::FileStatus_OK;
	EntityFactory* factory = static_cast<EntityFactory*>
		( m_world->getSystem( SystemType::EntityFactory ) );

	// Create Local rocks
//	status = factory->readAssemblageFile(LEVELPIECESPATH + "prisonChamberClient.asd" );
//	entity = factory->entityFromRecipe( "prisonChamberClient" );	
//
//	m_world->addEntity( entity );

	status = factory->readAssemblageFile( "Assemblages/testSpotLight.asd" );

	EntitySystem* tempSys = NULL;

	tempSys = m_world->getSystem(SystemType::GraphicsBackendSystem);
	GraphicsBackendSystem* graphicsBackend = static_cast<GraphicsBackendSystem*>(tempSys);

	/************************************************************************/
	/* Create the main camera used to render the scene						*/
	/************************************************************************/
	entity = m_world->createEntity();
	entity->addComponent( new CameraInfo( m_world->getAspectRatio(),1.047f,1.0f,1200.0f ) );
	entity->addComponent( new MainCamera_TAG() );
	entity->addComponent( new AudioListener() );
	entity->addComponent( new Transform( 0.0f, 0.0f, 0.0f ) );
	m_world->addEntity(entity);


	/************************************************************************/
	/* Create shadow camera and spotlight.									*/
	/************************************************************************/
	float rotation = 0.78;
	AglQuaternion quat;
	for(int i = 0; i < 1; i++){
		entity = factory->entityFromRecipe( "SpotLight" );
		LightsComponent* lightComp = static_cast<LightsComponent*>(
			entity->getComponent(ComponentType::LightsComponent));
		int shadowIdx = -1;
		vector<Light>* lights = lightComp->getLightsPtr();

		for (unsigned int i = 0; i < lights->size(); i++){
			if(lights->at(i).instanceData.shadowIdx != -1){
				shadowIdx = graphicsBackend->getGfxWrapper()->generateShadowMap();
				lights->at(i).instanceData.shadowIdx = shadowIdx;
			}
		}

		Transform* transform = static_cast<Transform*>(
			entity->getComponent(ComponentType::Transform));

		quat = AglQuaternion::constructFromAxisAndAngle(AglVector3::up(),rotation);
		transform->setRotation(quat);

		CameraInfo* cameraInfo = new CameraInfo(1);
		cameraInfo->m_shadowMapIdx = shadowIdx;
		entity->addComponent(ComponentType::CameraInfo, cameraInfo);
		entity->addTag(ComponentType::TAG_ShadowCamera, new ShadowCamera_TAG());
		m_world->addEntity( entity );

		rotation -= 0.78;
	}
	/*
	factory->readAssemblageFile("Assemblages/GlobalLight.asd");
	entity = factory->entityFromRecipe( "GlobalLight" );									 
	m_world->addEntity( entity );

	factory->readAssemblageFile("Assemblages/GreenLight.asd");
	entity = factory->entityFromRecipe( "GreenLight" );									 
	m_world->addEntity( entity );

	factory->readAssemblageFile("Assemblages/PinkLight.asd");
	entity = factory->entityFromRecipe( "PinkLight" );									 
	m_world->addEntity( entity );

	factory->readAssemblageFile("Assemblages/OrangeLight.asd");
	entity = factory->entityFromRecipe( "OrangeLight" );									 
	m_world->addEntity( entity );

	factory->readAssemblageFile("Assemblages/BlueLight.asd");
	entity = factory->entityFromRecipe( "BlueLight" );									 
	m_world->addEntity( entity );

	factory->readAssemblageFile("Assemblages/RedLight.asd");
	entity = factory->entityFromRecipe( "RedLight" );									 
	m_world->addEntity( entity );
	*/

	entity = m_world->createEntity();
	entity->addComponent(ComponentType::GameState,new GameState(MENU));
	m_world->addEntity(entity);

	//ParticleEmitters* ps = new ParticleEmitters();
	//AglParticleSystemHeader header;
	//header.particleSize = AglVector2(2, 2);
	//header.spawnFrequency = 10;
	//header.spawnSpeed = 5.0f;
	//header.spread = 0.0f;
	//header.fadeOutStart = 2.0f;
	//header.fadeInStop = 0.0f;
	//header.particleAge = 2;
	//header.maxOpacity = 1.0f;
	//header.color = AglVector4(0, 1, 0, 1.0f);
	//header.alignmentType = AglParticleSystemHeader::OBSERVER;
	//ps->addParticleSystem( header );
	//ps->getCollectionPtr()->m_particleSystems[0].setSpawnDirection( AglVector3(1.1f, 1.2f, 1.3f) );
	//ps->getCollectionPtr()->m_particleSystems[0].setSpawnSpeed( 2.340f );
	//ps->getCollectionPtr()->m_particleSystems[0].setMaxOpacity( 3.1f );
	//ps->getCollectionPtr()->m_particleSystems[0].setSpawnAngularVelocity( 45.6f );
	//ps->getCollectionPtr()->m_particleSystems[0].setSpread( 7.8f );
	//ps->getCollectionPtr()->m_particleSystems[0].setFadeOutStart( 8.9f );
	//ps->getCollectionPtr()->m_particleSystems[0].setFadeInStop( 0.1f );
	//ps->getCollectionPtr()->m_particleSystems[0].setSpawnPoint(AglVector3(-11.1f, -11.2f, -11.3f));
	//ps->getCollectionPtr()->m_particleSystems[0].setSpawnType(AglParticleSystemHeader::CONTINUOUSLY);
	//ps->getCollectionPtr()->m_particleSystems[0].setParticleSize(AglVector2(12.1f, 12.2f));
	//ps->getCollectionPtr()->m_particleSystems[0].setParticleAge( 13.1f );
	//ps->getCollectionPtr()->m_particleSystems[0].setSpawnFrequency( 14.1f );
	//entity->addComponent( ps );
}

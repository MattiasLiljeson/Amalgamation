#include "ClientApplication.h"
#include <windows.h>

#include "ServerApplication.h"

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
#include <SoundComponent.h>
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
#include <SettingsSystem.h>
#include <AntTweakBarEnablerSystem.h>
#include <AntTweakBarSystem.h>
#include <AudioBackendSystem.h>
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
#include <ClientStateSystem.h>
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
#include <LobbySystem.h>
#include <LevelInfoLoader.h>
#include <EnvironmentSystem.h>
#include <SpeedBufferUpdaterSystem.h>
#include <ShipParticleSystemUpdater.h>
#include <EditSphereSystem.h>
#include <SelectionMarkerSystem.h>
#include <InterpolationSystem2.h>
#include <AnomalyBombEffectSystem.h>
#include <ShieldPlaterSystem.h>
#include <SlotHighlightParticleMakerSystem.h>
#include <SpriteSystem.h>

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
#include <ModuleStatusEffectSystem.h>
#include <PlayerSystem.h>

#define FORCE_VS_DBG_OUTPUT


ClientApplication::ClientApplication( HINSTANCE p_hInstance )
{
		m_running = false;
		m_hInstance = p_hInstance;
		m_client = new TcpClient();
		m_world = new EntityWorld();

		m_serverApp = NULL;

		// Systems first!
		initSystems();

		// Test entities later!
		initEntities();
}

ClientApplication::~ClientApplication()
{
	if(m_serverApp != NULL){
		ProcessMessage* newMessage = new ProcessMessage(MessageType::TERMINATE,NULL);
		m_serverApp->putMessage( newMessage );
		m_serverApp->stop();
		delete m_serverApp;
	}

	delete m_world;
	delete m_client;
}

void ClientApplication::run()
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

			m_world->setDelta((float)dt);
			m_world->process();

			if(m_world->shouldShutDown()) {
				static_cast<SettingsSystem*>(
					m_world->getSystem( SystemType::SettingsSystem ) )->writeSettingsFile(SETTINGSPATH);
				m_running = false;
			}
			
			if(m_world->isHostingServer() && m_serverApp == NULL){
				m_serverApp = new Srv::ServerApplication();
				m_serverApp->start();
			}
			
		}

	}
}

void ClientApplication::initSystems()
{
	//----------------------------------------------------------------------------------
	// Systems must be added in the order they are meant to be executed. The order the
	// systems are added here is the order the systems will be processed
	//----------------------------------------------------------------------------------

	SettingsSystem* settingsSystem = new SettingsSystem();
	settingsSystem->readSettingsFile(SETTINGSPATH);
	GameSettingsInfo settings = settingsSystem->getSettings();
	m_world->setSystem( settingsSystem );

	/************************************************************************/
	/* TimerSystem used by other systems should be first.					*/
	/************************************************************************/
	m_world->setSystem(SystemType::TimerSystem, new TimerSystem(), true);

	/************************************************************************/
	/* Game State system.													*/
	/************************************************************************/
	m_world->setSystem( new ClientStateSystem( GameStates::MENU ) );

	/************************************************************************/
	/* PlayerSystem allows for accessing connected players					*/
	/************************************************************************/
	m_world->setSystem( new PlayerSystem(), true);

	/************************************************************************/
	/* Graphics																*/
	/************************************************************************/
	GraphicsBackendSystem* graphicsBackend = new GraphicsBackendSystem( m_hInstance,
		settings );

	m_world->setSystem( graphicsBackend );

	/************************************************************************/
	/* Entity creation														*/
	/************************************************************************/
	m_world->setSystem( new EntityFactory(m_client, NULL) );

	/************************************************************************/
	/* Level handling														*/
	/************************************************************************/
	m_world->setSystem( new LevelHandlerSystem() );
	m_world->setSystem( new LevelInfoLoader() );

	/************************************************************************/
	/* Mesh loading															*/
	/************************************************************************/
	// Note! Must set *after* EntityFactory and GraphicsBackend, and *before* Physics
	m_world->setSystem( new LoadMeshSystemClient(graphicsBackend) ); 
	m_world->setSystem( new ParticleSystemInstructionTranslatorSystem( graphicsBackend ) );
	/************************************************************************/
	/* Physics																*/
	/************************************************************************/
	PhysicsSystem* physics = new PhysicsSystem( NULL );
	m_world->setSystem( physics );

	/************************************************************************/
	/* General controlling													*/
	/************************************************************************/
	m_world->setSystem( new LookAtSystem(NULL) );
	m_world->setSystem( new SpeedBufferUpdaterSystem() );
	
	/************************************************************************/
	/* Input																*/
	/************************************************************************/
	InputBackendSystem* inputBackend = new InputBackendSystem( m_hInstance, 
		graphicsBackend );
	m_world->setSystem( inputBackend );
	m_world->setSystem( new InputActionsBackendSystem( SETTINGSPATH + "input.ini" ) );

	GamepadRumbleSystem* rumbleSys = new GamepadRumbleSystem( inputBackend );
	rumbleSys->setRumbleEnabled( settings.rumble );
	m_world->setSystem( rumbleSys );

	/************************************************************************/
	/* Culling																*/
	/************************************************************************/
	m_world->setSystem(new CullingSystem() );

	
	/************************************************************************/
	/* GUI																	*/
	/************************************************************************/
	LibRocketBackendSystem* rocketBackend = new LibRocketBackendSystem( graphicsBackend,
		inputBackend );
	m_world->setSystem( rocketBackend );

	m_world->setSystem( new LobbySystem() );
	m_world->setSystem( new HudSystem( rocketBackend ) );
	m_world->setSystem( new LibRocketEventManagerSystem(m_client) );
	m_world->setSystem( new GameOptionsSystem() );

	// NOTE: MenuSystem looks up all systems that's also deriving from EventHandler, so
	// that they can be properly be added to the LibRocketEventManager.
	// The alternative would be that every event handler adds itself.
	m_world->setSystem( new MenuSystem() );


	/************************************************************************/
	/* Effects																*/
	/************************************************************************/
	m_world->setSystem( new SlotHighlightParticleMakerSystem() );
	m_world->setSystem( new ScoreWorldVisualizerSystem() );
	m_world->setSystem( new ModuleStatusEffectSystem() );
	m_world->setSystem( new ConnectionVisualizerSystem() );
	m_world->setSystem( new ShipParticleSystemUpdater() );
	m_world->setSystem( new EditSphereSystem() );
	m_world->setSystem( new SelectionMarkerSystem());

	/************************************************************************/
	/* Player    															*/
	/************************************************************************/
	// Input system for ships
	ShipInputProcessingSystem* shipInputProc = new ShipInputProcessingSystem(inputBackend);
	m_world->setSystem( shipInputProc );
	SlotInputControllerSystem* slotInput = new SlotInputControllerSystem(inputBackend, m_client);
	m_world->setSystem( slotInput );

	// Controller systems for the ship
	m_world->setSystem( new ShipFlyControllerSystem(shipInputProc, physics, m_client, slotInput ));
	m_world->setSystem( new ShipEditControllerSystem(shipInputProc, physics, slotInput) );

	/************************************************************************/
	/* Hierarchy															*/
	/************************************************************************/
	m_world->setSystem( new EntityParentHandlerSystem() );


	/************************************************************************/
	/* Camera																*/
	/************************************************************************/
	// Chamber fog and ambient
	m_world->setSystem( new EnvironmentSystem() );

	// Controller logic for camera
	m_world->setSystem( new PlayerCameraControllerSystem( shipInputProc, m_client ) );
	// Camera system sets its viewport info to the graphics backend for render
	m_world->setSystem( new CameraSystem( graphicsBackend ) );

	m_world->setSystem( new SpeedFovAdjustSystem() );

	/************************************************************************/
	/* Sprites																*/
	/************************************************************************/
	SpriteSystem* spriteSystem = new SpriteSystem();
	m_world->setSystem(spriteSystem);

	/************************************************************************/
	/* Renderer																*/
	/************************************************************************/
	MeshRenderSystem* renderer = new MeshRenderSystem( graphicsBackend );
	m_world->setSystem( renderer );

	ParticleRenderSystem* particleRender = new ParticleRenderSystem( graphicsBackend );
	m_world->setSystem( particleRender );

	LightRenderSystem* lightRender = new LightRenderSystem( graphicsBackend );
	m_world->setSystem( lightRender );
	
	AntTweakBarSystem* antTweakBar = new AntTweakBarSystem( graphicsBackend, inputBackend );
	m_world->setSystem( antTweakBar, false );

	ShadowSystem* shadowSystem = new ShadowSystem ();
	m_world->setSystem( shadowSystem );

	/************************************************************************/
	/* Network																*/
	/************************************************************************/
	m_world->setSystem( new ProcessingMessagesSystem( m_client ) );
	m_world->setSystem( new ClientConnectToServerSystem( m_client, false ) );
	m_world->setSystem( new NetsyncDirectMapperSystem() );
	m_world->setSystem( new NetSyncedPlayerScoreTrackerSystem() );
	m_world->setSystem( new ClientPacketHandlerSystem( m_client ) );
	m_world->setSystem( new ExtrapolationSystem( m_client ) );

	/************************************************************************/
	/* Interpolation  														*/
	/************************************************************************/
	// InterpolationSystem* interpolationSystem = new InterpolationSystem();
	// m_world->setSystem( interpolationSystem, true);
	InterpolationSystem2* inter = new InterpolationSystem2();
	m_world->setSystem(inter, true);

	/************************************************************************/
	/* Audio																*/
	/************************************************************************/
	AudioBackendSystem* audioBackend = new AudioBackendSystem();
	m_world->setSystem( audioBackend );
	m_world->setSystem( new AudioListenerSystem(audioBackend) );
	m_world->setSystem( new PositionalSoundSystem() );

	/************************************************************************/
	/* Gameplay																*/
	/************************************************************************/
	m_world->setSystem( new DisplayPlayerScoreSystem(m_client) );
	m_world->setSystem( new ClientPickingSystem(m_client) );
	m_world->setSystem( new GameStatsSystem() );
	m_world->setSystem( new LightBlinkerSystem() );
	m_world->setSystem( new ShieldPlatingSystem() );

	/************************************************************************/
	/* Animation															*/
	/************************************************************************/
	m_world->setSystem( new SkeletalAnimationSystem() );

	/************************************************************************/
	/* Graphics representer													*/
	/************************************************************************/
	m_world->setSystem( new GraphicsRendererSystem( graphicsBackend, shadowSystem,
		renderer, rocketBackend, particleRender, antTweakBar, lightRender) );

	/************************************************************************/
	/* Destroyers															*/
	/************************************************************************/
	m_world->setSystem( new DestroyOnParticlesDeathSystem() );

	/************************************************************************/
	/* Debugging															*/
	/************************************************************************/
	m_world->setSystem( new DebugMovementSystem(), false );
	m_world->setSystem( new MenuBackgroundSceneSystem() );
	m_world->setSystem( new CircularMovementSystem() );
	m_world->setSystem( new AxisRotationSystem() );
	m_world->setSystem( new MoveShipLightsSystem() );
	m_world->setSystem( new ClientMeasurementSystem() );
	m_world->setSystem( new ClientEntityCountSystem() );
	m_world->setSystem( new AntTweakBarEnablerSystem() );
	m_world->setSystem( new AnomalyBombEffectSystem() );
	m_world->setSystem( new ShieldPlaterSystem() );

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

	status = factory->readAssemblageFile( "Assemblages/testSpotLight.asd" );

	EntitySystem* tempSys = NULL;

	tempSys = m_world->getSystem(SystemType::GraphicsBackendSystem);
	GraphicsBackendSystem* graphicsBackend = static_cast<GraphicsBackendSystem*>(tempSys);

	/************************************************************************/
	/* Create the main camera used to render the scene						*/
	/************************************************************************/
	entity = m_world->createEntity();
	entity->addComponent( new CameraInfo( m_world->getAspectRatio(),1.3f,1.0f,3000.0f ) );
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
}

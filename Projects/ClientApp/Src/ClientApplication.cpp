#include "ClientApplication.h"
#include <windows.h>

#include <SettingsSystem.h>

#include "ServerApplication.h"

#include <EntityWorld.h>
#include <Input.h>
#include <ComponentAssemblageAllocator.h>

// Components
#include <AudioInfo.h>
#include <AudioListener.h>
#include <BodyInitData.h>
#include <OrbitalMovement.h>
#include <ConnectionPointSet.h>
#include <Connector1to2Module.h>
#include <DebugMove.h>
#include <EntityParent.h>
#include <GameState.h>
#include <GameplayTags.h>
#include <HudElement.h>
#include <InterpolationComponent.h>
#include <LightsComponent.h>
#include <LoadMesh.h>
#include <MineLayerModule.h>
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

// Systems
#include <AnomalyBombEffectSystem.h>
#include <AntTweakBarEnablerSystem.h>
#include <AntTweakBarSystem.h>
#include <AudioBackendSystem.h>
#include <AudioListenerSystem.h>
#include <AxisRotationSystem.h>
#include <CameraInfo.h>
#include <CameraSystem.h>
#include <OrbitalMovementSystem.h>
#include <ClientConnectToServerSystem.h>
#include <ClientEntityCountSystem.h>
#include <ClientMeasurementSystem.h>
#include <ClientPacketHandlerSystem.h>
#include <ClientPickingSystem.h>
#include <ClientStateSystem.h>
#include <ConnectionVisualizerSystem.h>
#include <CullingSystem.h>
#include <DamageUpdaterSystem.h>
#include <DamageVisualizerSystem.h>
#include <DebugMovementSystem.h>
#include <DisplayPlayerScoreSystem.h>
#include <EditSphereSystem.h>
#include <EntityFactory.h>
#include <EnvironmentSystem.h>
#include <ExtrapolationSystem.h>
#include <GameStatsSystem.h>
#include <GamepadRumbleSystem.h>
#include <GraphicsBackendSystem.h>
#include <GraphicsRendererSystem.h>
#include <HudSystem.h>
#include <InputActionsBackendSystem.h>
#include <InputBackendSystem.h>
#include <InterpolationSystem.h>
#include <InterpolationSystem2.h>
#include <LevelGenSystem.h>
#include <LevelHandlerSystem.h>
#include <LevelInfoLoader.h>
#include <LibRocketBackendSystem.h>
#include <LibRocketEventManagerSystem.h>
#include <LightBlinkerSystem.h>
#include <LightRenderSystem.h>
#include <LoadMeshSystemClient.h>
#include <LobbySystem.h>
#include <LookAtEntity.h>
#include <LookAtSystem.h>
#include <MenuBackgroundSceneSystem.h>
#include <MenuSystem.h>
#include <MeshRenderSystem.h>
#include <MineControllerSystem.h>
#include <MineLayerModuleControllerSystem.h>
#include <MinigunModuleControllerSystem.h>
#include <MoveShipLightsSystem.h>
#include <NetSyncedPlayerScoreTrackerSystem.h>
#include <NetsyncDirectMapperSystem.h>
#include <OutputLogger.h>
#include <ParticleRenderSystem.h>
#include <ParticleSystemInstructionTranslatorSystem.h>
#include <PhysicsSystem.h>
#include <PlayerCameraControllerSystem.h>
#include <PositionalSoundSystem.h>
#include <ProcessingMessagesSystem.h>
#include <RocketLauncherModuleControllerSystem.h>
#include <ScoreWorldVisualizerSystem.h>
#include <SelectionMarkerSystem.h>
#include <ShadowSystem.h>
#include <ShieldModuleControllerSystem.h>
#include <ShieldEffectSystem.h>
#include <ShieldPlatingSystem.h>
#include <ShipEditControllerSystem.h>
#include <ShipFlyControllerSystem.h>
#include <ShipInputProcessingSystem.h>
#include <ShipModulesControllerSystem.h>
#include <ShipParticleSystemUpdater.h>
#include <SkeletalAnimationSystem.h>
#include <SlotHighlightParticleMakerSystem.h>
#include <SpeedBufferUpdaterSystem.h>
#include <SpeedFovAdjustSystem.h>
#include <SlotMarkerSystem.h>
#include <TeslaEffectSystem.h>
#include <TeslaLightningSystem.h>
#include <TimerSystem.h>
#include <TransformParentHandlerSystem.h>
#include <ShipHiglightSystem.h>
#include <ModuleHighlightSystem.h>
#include <PortalCullingSystem.h>
#include <ClientModuleCounterSystem.h>
#include <AddToParentSystem.h>
#include <GlowAnimationSystem.h>
#include <ClientDebugModuleSpawnerSystem.h>
#include <PreloaderSystem.h>
#include <EntityLoggerSystem.h>
#include <SineMovementSystem.h>

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
#include <StateManagementSystem.h>
#include <ShipManagerSystem.h>

// unsorted includes. Sort these as soon as they're added!
#include <PlayerSystem.h>
#include <SoundSystem.h>
#include <ShipEngineSystem.h>
#include <ValueClamp.h>
#include <FadeInSystem.h>


#define FORCE_VS_DBG_OUTPUT


ClientApplication::ClientApplication( HINSTANCE p_hInstance )
{

	RandomUtil::seed();
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

			dt = clamp(dt,0.0,DTCAP);

			m_world->setDelta((float)dt);
			m_world->process();

			if(m_world->shouldShutDown()) {
				static_cast<SettingsSystem*>(
					m_world->getSystem( SystemType::SettingsSystem ) )
					->writeSettingsFile(SETTINGSPATH);
				m_running = false;
			}
			else if (m_world->shouldRestart()) {
				static_cast<SettingsSystem*>(
					m_world->getSystem( SystemType::SettingsSystem ) )
					->writeSettingsFile(SETTINGSPATH);
				static_cast<GraphicsRendererSystem*>
					(m_world->getSystem(SystemType::GraphicsRendererSystem))
					->printLogFiles();

				delete m_world;
				delete m_client;

				m_client = new TcpClient();
				m_world = new EntityWorld();

				//m_serverApp = NULL;

				// Systems first!
				initSystems();

				// Test entities later!
				initEntities();
				//m_running = false;
			}
			
			if(m_world->isHostingServer() && m_serverApp == NULL){
				auto clientConnectToServerSystem =  static_cast<ClientConnectToServerSystem*>
					(m_world->getSystem(SystemType::ClientConnectoToServerSystem));

				m_serverApp = new Srv::ServerApplication( m_world->getServerGameTime(),
					clientConnectToServerSystem->getServerPortByInt(),
					m_world->getServerName());
				m_serverApp->start();
			}
			else if (!m_world->isHostingServer() && m_serverApp != NULL){
				ProcessMessage* newMessage = new ProcessMessage(MessageType::TERMINATE,NULL);
				m_serverApp->putMessage( newMessage );
				m_serverApp->stop();
				delete m_serverApp;
				m_serverApp = NULL;
			}
		}

	}
	delete GraphicsBackendSystem::getWindow();
}

void ClientApplication::initSystems()
{
	//----------------------------------------------------------------------------------
	// Systems must be added in the order they are meant to be executed. The order the
	// systems are added here is the order the systems will be processed
	//----------------------------------------------------------------------------------

	/************************************************************************/
	/* ---------------------THE NEW WORLD ORDER!--------------------------	*/
	/************************************************************************/
	//SYSTEMS USED BY MANY IN CREATIONSTAGES
	SettingsSystem* settingsSystem = new SettingsSystem();
	settingsSystem->readSettingsFile(SETTINGSPATH);
	GameSettingsInfo settings = settingsSystem->getSettings();
	m_world->setSystem( settingsSystem );

	GraphicsBackendSystem* graphicsBackend = new GraphicsBackendSystem( m_hInstance,
		settings );
	m_world->setSystem( graphicsBackend );
	m_world->setSystem( new NetsyncDirectMapperSystem() );
	AudioBackendSystem* audioBackend = new AudioBackendSystem();
	m_world->setSystem( audioBackend );

	// || INPUT ||
	InputBackendSystem* inputBackend = new InputBackendSystem( m_hInstance, 
		graphicsBackend );
	m_world->setSystem( inputBackend );

	InputActionsBackendSystem* actionBackendSys =
		new InputActionsBackendSystem( SETTINGSPATH, "settings.input" );
	m_world->setSystem( actionBackendSys );

	LibRocketBackendSystem* rocketBackend = new LibRocketBackendSystem( graphicsBackend,
		inputBackend, actionBackendSys );
	m_world->setSystem( rocketBackend );
	m_world->setSystem( new LibRocketEventManagerSystem(m_client) );

	//---NETWORK
	m_world->setSystem( new ProcessingMessagesSystem( m_client ) );
	//m_world->setSystem( new NetSyncedPlayerScoreTrackerSystem() );
	//m_world->setSystem( new ExtrapolationSystem( m_client ) );

	// || UPDATE SYSTEMS ||
	m_world->setSystem(new TimerSystem());

	//---SHIP CONTROLLING SYSTEMS
	ShipInputProcessingSystem* shipInputProc = new ShipInputProcessingSystem(inputBackend);
	m_world->setSystem( shipInputProc );
	SlotInputControllerSystem* slotInput = new SlotInputControllerSystem(inputBackend,
		m_client);
	m_world->setSystem( slotInput );
	m_world->setSystem( new ShipFlyControllerSystem(shipInputProc, NULL, m_client, slotInput ));
	m_world->setSystem( new ShipEditControllerSystem(shipInputProc, NULL, slotInput) );
	m_world->setSystem( new PlayerCameraControllerSystem( shipInputProc, m_client ) );
	m_world->setSystem( new ShipManagerSystem());

	//---NETWORKHANDLING SYSTEMS
	m_world->setSystem( new ClientPacketHandlerSystem( m_client ) );

	//---GAMEPLAY AFFECTING SYSTEMS
	m_world->setSystem( new LookAtSystem(NULL) );
	m_world->setSystem( new SpeedBufferUpdaterSystem() );
	m_world->setSystem( new DamageUpdaterSystem() );
	m_world->setSystem( new GamepadRumbleSystem( inputBackend, settings ) );

	//---EFFECTS
	m_world->setSystem( new ScoreWorldVisualizerSystem() );
	//m_world->setSystem( new ModuleStatusEffectSystem() );
	m_world->setSystem( new ConnectionVisualizerSystem() );
	m_world->setSystem( new ShipParticleSystemUpdater() );
	m_world->setSystem( new EditSphereSystem() );
	m_world->setSystem( new SelectionMarkerSystem() );
	m_world->setSystem( new ShipHighlightSystem() );
	m_world->setSystem( new ModuleHighlightSystem() );
	m_world->setSystem( new DamageVisualizerSystem() );
	m_world->setSystem( new EnvironmentSystem() );	// Chamber fog and ambient
	m_world->setSystem( new SpeedFovAdjustSystem() );
	m_world->setSystem( new ShipEngineSystem() );

	//---GAMEPLAY
	m_world->setSystem( new PlayerSystem(), true);
	m_world->setSystem( new DisplayPlayerScoreSystem(m_client) );
	m_world->setSystem( new ClientPickingSystem(m_client) );
	m_world->setSystem( new GameStatsSystem() );
	m_world->setSystem( new LightBlinkerSystem() );
	m_world->setSystem( new ShieldEffectSystem() );
	m_world->setSystem( new ShieldPlatingSystem() );
	m_world->setSystem( new SlotMarkerSystem() );
	m_world->setSystem( new AnomalyBombEffectSystem() );
	m_world->setSystem( new TeslaEffectSystem() );
	m_world->setSystem( new TeslaLightningSystem() );
	m_world->setSystem( new GlowAnimationSystem() );

	//---STATESYSTEM
	m_world->setSystem( new ClientStateSystem( GameStates::MENU ) );
	m_world->setSystem( new ClientConnectToServerSystem( m_client, false ) );
	m_world->setSystem( new StateManagementSystem(m_client));

	//---GUI UPDATE SYSTEMS
	m_world->setSystem( new LobbySystem() );
	m_world->setSystem( new HudSystem( rocketBackend, m_client ) );
	// NOTE: MenuSystem looks up all systems that's also deriving from EventHandler, so
	// that they can be properly be added to the LibRocketEventManager.
	// The alternative would be that every event handler adds itself.
	m_world->setSystem( new MenuSystem( m_client ) );

	//IS THIS SYSTEM USEEEEEEDDD????????-Robin
	// InterpolationSystem* interpolationSystem = new InterpolationSystem();
	// m_world->setSystem( interpolationSystem, true);
	InterpolationSystem2* inter = new InterpolationSystem2();
	m_world->setSystem(inter, true);
	m_world->setSystem( new EntityParentHandlerSystem() );

	//---AUDIO SYSTEMS
	m_world->setSystem( new FadeInSystem() );
	m_world->setSystem( new AudioListenerSystem(audioBackend) );
	m_world->setSystem( new PositionalSoundSystem() );
	m_world->setSystem( new SoundSystem(audioBackend) );

	// || CREATION THINGS ||
	/************************************************************************/
	/* Entity Creation														*/
	/************************************************************************/
	m_world->setSystem( new EntityFactory(m_client, NULL) );
	m_world->setSystem( new PreloaderSystem() );

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

	m_world->setSystem( new ModuleStatusEffectSystem() );

	// || RENDERING SYSTEMS ||	
	m_world->setSystem( new CameraSystem( graphicsBackend ) );
	m_world->setSystem( new SkeletalAnimationSystem() );

	//---CULLING
	m_world->setSystem(new AddToParentSystem());
	m_world->setSystem(new PortalCullingSystem());
	m_world->setSystem(new CullingSystem() );

	//---RENDERSYSTEMS
	MeshRenderSystem* meshRender = new MeshRenderSystem( graphicsBackend );
	m_world->setSystem( meshRender );
	ParticleRenderSystem* particleRender = new ParticleRenderSystem( graphicsBackend );
	m_world->setSystem( particleRender );
	LightRenderSystem* lightRender = new LightRenderSystem( graphicsBackend );
	m_world->setSystem( lightRender );
	AntTweakBarSystem* antTweakBar = new AntTweakBarSystem( graphicsBackend, inputBackend );
	m_world->setSystem( antTweakBar, false );

	m_world->setSystem( new GraphicsRendererSystem( graphicsBackend, meshRender, 
		rocketBackend, particleRender, antTweakBar, lightRender, settings ) );

	/************************************************************************/
	/* Destroyers															*/
	/************************************************************************/
	m_world->setSystem( new DestroyOnParticlesDeathSystem() );

	/************************************************************************/
	/* Debugging															*/
	/************************************************************************/
	m_world->setSystem( new DebugMovementSystem(), false );
	m_world->setSystem( new MenuBackgroundSceneSystem()); // NOTE: Used for menu background! // NOTE: As the name implies! :D
	m_world->setSystem( new OrbitalMovementSystem() );
	m_world->setSystem( new AxisRotationSystem() );
	m_world->setSystem( new SineMovementSystem() );
	m_world->setSystem( new MoveShipLightsSystem() );
	m_world->setSystem( new ClientMeasurementSystem() );
	m_world->setSystem( new ClientEntityCountSystem() );
	m_world->setSystem( new AntTweakBarEnablerSystem() );
	m_world->setSystem( new OutputLogger("log_client.txt"));
	m_world->setSystem( new ClientModuleCounterSystem() ); // NOTE: Used to be able to remove modules!
	m_world->setSystem( new ClientDebugModuleSpawnerSystem(m_client) );
	m_world->setSystem( new EntityLoggerSystem(), false );


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
	entity->setName("MainCamera");
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

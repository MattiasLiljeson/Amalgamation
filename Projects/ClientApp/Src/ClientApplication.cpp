#include "ClientApplication.h"
#include <windows.h>

#ifdef COMBINE_CLIENT_AND_SERVER
	#include "ServerApplication.h"
#endif

#include <EntityWorld.h>
#include <Input.h>

// Components
#include <AudioInfo.h>
#include <AudioListener.h>
#include <BodyInitData.h>
#include <PhysicsBody.h>
#include <PhysicsSystem.h>
#include <RenderInfo.h>
#include <ShipFlyController.h>
#include <ShipEditController.h>
#include <Transform.h>
#include <HudElement.h>
#include <ShipModule.h>
#include <ConnectionPointSet.h>
#include <SpeedBoosterModule.h>
#include <MinigunModule.h>
#include <GameplayTags.h>
#include <PlayerCameraController.h>


// Systems
#include <AudioBackendSystem.h>
#include <AudioController.h>
#include <AudioListenerSystem.h>
#include <CameraSystem.h>
#include <GraphicsBackendSystem.h>
#include <InputBackendSystem.h>
#include <LibRocketBackendSystem.h>
#include <ClientPacketHandlerSystem.h>
#include <NetworkConnectToServerSystem.h>
#include <PhysicsSystem.h>
#include <ProcessingMessagesSystem.h>
#include <RenderPrepSystem.h>
#include <ShipFlyControllerSystem.h>
#include <ShipEditControllerSystem.h>
#include <ShipInputProcessingSystem.h>
#include <DisplayPlayerScoreSystem.h>
#include <LookAtSystem.h>
#include <HudSystem.h>
#include <CameraInfo.h>
#include <LookAtEntity.h>
#include <MainCamera.h>
#include <MinigunModuleControllerSystem.h>
#include <PlayerCameraControllerSystem.h>

// Helpers
#include <ConnectionPointCollection.h>
#include <vector>

using namespace std;

// MISC
#include <AntTweakBarWrapper.h>



ClientApplication::ClientApplication( HINSTANCE p_hInstance )
{
	try{
		m_running = false;
		m_hInstance = p_hInstance;
		m_client = new TcpClient();
		m_world = new EntityWorld();

#ifdef COMBINE_CLIENT_AND_SERVER
		m_serverApp = new Srv::ServerApplication();
#endif

		initSystems();
		initEntities();

#ifdef ENABLE_SOUND
		initSoundSystem();
		initSounds();
#endif

	}
	catch(exception& e)
	{
		DEBUGWARNING((e.what()));
	}
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
	while(WM_QUIT != msg.message)
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
	/* Physics																*/
	/************************************************************************/
	PhysicsSystem* physics = new PhysicsSystem();
	m_world->setSystem(SystemType::PhysicsSystem, physics, true);

	/************************************************************************/
	/* General controlling													*/
	/************************************************************************/
	LookAtSystem* lookAtSystem = new LookAtSystem();
	m_world->setSystem(SystemType::LookAtSystem, lookAtSystem, true);
	
	/************************************************************************/
	/* Graphics																*/
	/************************************************************************/
	GraphicsBackendSystem* graphicsBackend = new GraphicsBackendSystem( m_hInstance ,
		1280,720,true);

	m_world->setSystem( graphicsBackend, true );

	InputBackendSystem* inputBackend = new InputBackendSystem( m_hInstance, graphicsBackend );
	m_world->setSystem( inputBackend, true);

	LibRocketBackendSystem* rocketBackend = new LibRocketBackendSystem( graphicsBackend,
		inputBackend );
	m_world->setSystem( rocketBackend, true );

	HudSystem* hud = new HudSystem( rocketBackend );
	m_world->setSystem( hud, true );

	/************************************************************************/
	/* Player    															*/
	/************************************************************************/
	// Input system for ships
	ShipInputProcessingSystem* shipInputProc = new ShipInputProcessingSystem(inputBackend);
	m_world->setSystem( shipInputProc, true);

	// Controller systems for the ship
	ShipFlyControllerSystem* shipFlyController = new ShipFlyControllerSystem(shipInputProc, physics,
		m_client );
	m_world->setSystem( shipFlyController, true);

	ShipEditControllerSystem* shipEditController = new ShipEditControllerSystem(shipInputProc, physics/*,
		m_client*/ );
	m_world->setSystem( shipEditController, true);


	/************************************************************************/
	/* Camera																*/
	/************************************************************************/

	// Controller logic for camera
	PlayerCameraControllerSystem* cameraControl = new PlayerCameraControllerSystem( shipInputProc );
	m_world->setSystem( cameraControl , true );
	// Camera system sets its viewport info to the graphics backend for render
	CameraSystem* camera = new CameraSystem( graphicsBackend );
	m_world->setSystem( camera , true );

	RenderPrepSystem* renderer = new RenderPrepSystem( graphicsBackend, rocketBackend );
	m_world->setSystem( renderer , true );

	/************************************************************************/
	/* Network																*/
	/************************************************************************/
	/*ProcessingMessagesSystem* msgProcSystem = new ProcessingMessagesSystem( m_client );
	m_world->setSystem( msgProcSystem , true );

	NetworkConnectToServerSystem* connect =
		new NetworkConnectToServerSystem( m_client, inputBackend );
	m_world->setSystem( connect, true );

	ClientPacketHandlerSystem* communicatorSystem =
		new ClientPacketHandlerSystem( m_client );
	m_world->setSystem( communicatorSystem, false );*/

	/************************************************************************/
	/* Audio															*/
	/************************************************************************/
	AudioBackendSystem* audioBackend = new AudioBackendSystem();
	m_world->setSystem( SystemType::AudioBackendSystem, audioBackend, true);

	AudioController* audioController = new AudioController(audioBackend);
	m_world->setSystem( SystemType::AudioControllerSystem, audioController, true);

	AudioListenerSystem* audioListener = new AudioListenerSystem(audioBackend);
	m_world->setSystem( SystemType::AudioListenerSystem, audioListener, true);

	/************************************************************************/
	/* Gameplay																 */
	/************************************************************************/
	m_world->setSystem( new DisplayPlayerScoreSystem(), true );
	m_world->setSystem(new MinigunModuleControllerSystem(), true);
	
	m_world->initialize();
}

void ClientApplication::initEntities()
{
	Entity* entity;
	Component* component;

	EntitySystem* tempSys = NULL;

	// Load cube model used as graphic representation for all "graphical" entities.
	tempSys = m_world->getSystem(SystemType::GraphicsBackendSystem);
	GraphicsBackendSystem* graphicsBackend = static_cast<GraphicsBackendSystem*>(tempSys);
	int cubeMeshId = graphicsBackend->createMesh( "P_cube" );
	int shipMeshId = graphicsBackend->createMesh( "Ship.agl", &TESTMODELPATH );
	int sphereMeshId = graphicsBackend->createMesh( "P_sphere" );

	ConnectionPointCollection connectionPoints;
	int testchamberId = graphicsBackend->createMesh( "test_parts_3sphere.agl", 
													 &TESTMODELPATH,
													 &connectionPoints);

	// Testchamber
	entity = m_world->createEntity();
	component = new RenderInfo( testchamberId );
	entity->addComponent( ComponentType::RenderInfo, component );
	component = new Transform( 5.0f, 10.0f, 19.0f);
	entity->addComponent( ComponentType::Transform, component );
	m_world->addEntity(entity);



	// Create a "spaceship"
	entity = m_world->createEntity();
	int shipId = entity->getIndex();
	component = new RenderInfo( shipMeshId );
	entity->addComponent( ComponentType::RenderInfo, component );
	component = new Transform(0, 0, 0);
	entity->addComponent( ComponentType::Transform, component );

	entity->addComponent( ComponentType::PhysicsBody, 
		new PhysicsBody() );

	entity->addComponent( ComponentType::BodyInitData, 
		new BodyInitData(AglVector3(0, 0, 0),
		AglQuaternion::identity(),
		AglVector3(1, 1, 1), AglVector3(0, 0, 0), 
		AglVector3(0, 0, 0), 0, 
		BodyInitData::DYNAMIC, 
		BodyInitData::COMPOUND));


	component = new ShipFlyController(5.0f, 50.0f);
	entity->addComponent( ComponentType::ShipFlyController, component );

	component = new ShipEditController();
	entity->addComponent( ComponentType::ShipEditController, component);

	// default tag is fly
	entity->addTag(ComponentType::TAG_ShipFlyMode, new ShipFlyMode_TAG());

	ConnectionPointSet* connectionPointSet = new ConnectionPointSet();
	connectionPointSet->m_connectionPoints.push_back(ConnectionPoint(AglMatrix::createTranslationMatrix(AglVector3(2.5f, 0, 0))));
	connectionPointSet->m_connectionPoints.push_back(ConnectionPoint(AglMatrix::createTranslationMatrix(AglVector3(-2.5f, 0, 0))));
	connectionPointSet->m_connectionPoints.push_back(ConnectionPoint(AglMatrix::createTranslationMatrix(AglVector3(0, 2.5f, 0))));

	entity->addComponent(ComponentType::ConnectionPointSet, connectionPointSet);

	m_world->addEntity(entity);


	InitModulesTestByAnton();

	// Add a grid of cubes to test instancing.
	for( int x=0; x<8; x++ )
	{
		for( int y=0; y<8; y++ )
		{
			for( int z=0; z<8; z++ )
			{
				entity = m_world->createEntity();
				component = new RenderInfo( cubeMeshId );
				entity->addComponent( ComponentType::RenderInfo, component );
				component = new Transform( AglVector3(.0f+5.0f*-x, 1.0f+5.0f*-y, 1.0f+5.0f*-z),
											AglQuaternion::identity(), 
										   AglVector3(1.0f,1.0f,5.0f)
										   );
				entity->addComponent( ComponentType::Transform, component );
				component = new LookAtEntity(shipId);
				entity->addComponent( ComponentType::LookAtEntity, component );
				m_world->addEntity(entity);
			}
		}

	}

	//Create a camera
	float aspectRatio = 
		static_cast<GraphicsBackendSystem*>(m_world->getSystem(
		SystemType::GraphicsBackendSystem ))->getAspectRatio();

	entity = m_world->createEntity();
	component = new CameraInfo( aspectRatio );
	entity->addComponent( ComponentType::CameraInfo, component );
	component = new MainCamera();
	entity->addComponent( ComponentType::MainCamera, component );
	//component = new Input();
	//entity->addComponent( ComponentType::Input, component );
	component = new Transform( -5.0f, 0.0f, -5.0f );
	entity->addComponent( ComponentType::Transform, component );
	component = new LookAtEntity(shipId, 
								 AglVector3(0,3,-10),
								 AglQuaternion::identity(),
								 10.0f,
								 10.0f,
								 4.0f);
	entity->addComponent( ComponentType::LookAtEntity, component );
	// default tag is follow
	entity->addTag(ComponentType::TAG_LookAtFollowMode, new LookAtFollowMode_TAG() );
	entity->addComponent(ComponentType::PlayerCameraController, new PlayerCameraController() );
	component = new AudioListener();
	entity->addComponent(ComponentType::AudioListener, component);
	
	m_world->addEntity(entity);
}

void ClientApplication::initSounds()
{

	EntitySystem*	tempSys			= NULL;
	Entity*			entity			= NULL;
	Component*		component		= NULL;
	int				soundIdx		= -1;
	string			fullFilePath;
	string			file;

	tempSys = m_world->getSystem(SystemType::AudioBackendSystem);
	AudioBackendSystem* audioBackend = static_cast<AudioBackendSystem*>(tempSys);


	/************************************************************************/
	/* Load positional sound												*/
	/************************************************************************/
	file = "Techno_1.wav";
	BasicSoundCreationInfo basicSoundInfo = BasicSoundCreationInfo(file.c_str(),
		TESTMUSICPATH.c_str(),true);
	PositionalSoundCreationInfo positionalSoundInfo = PositionalSoundCreationInfo(
		AglVector3( 3.0f, -10.0f, -30.0f ));
	soundIdx = audioBackend->createPositionalSound(&basicSoundInfo,&positionalSoundInfo);
	entity = m_world->createEntity();
	component = new Transform( 3.0f, -10.0f, -30.0f );
	entity->addComponent( ComponentType::Transform, component );
	component = new AudioInfo(soundIdx,true);
	entity->addComponent(ComponentType::AudioComponent, component);
	m_world->addEntity(entity);
	audioBackend->changeAudioInstruction(soundIdx, SoundEnums::Instructions::PLAY);

	/************************************************************************/
	/* Load positional sound												*/
	/************************************************************************/
	file = "MusicMono.wav";
	basicSoundInfo = BasicSoundCreationInfo(file.c_str(), TESTMUSICPATH.c_str(),true);
	positionalSoundInfo = PositionalSoundCreationInfo( AglVector3(3,3,3) );
	soundIdx = audioBackend->createPositionalSound(&basicSoundInfo,&positionalSoundInfo);
	entity = m_world->createEntity();
	component = new Transform( 3.0f, 3.0f, 3.0f );
	entity->addComponent( ComponentType::Transform, component );
	component = new AudioInfo(soundIdx,true);
	entity->addComponent(ComponentType::AudioComponent, component);
	m_world->addEntity(entity);
	audioBackend->changeAudioInstruction(soundIdx, SoundEnums::Instructions::PLAY);

	/************************************************************************/
	/* Load ambient sound													*/
	/************************************************************************/
	file = "Techno_1.wav";
	basicSoundInfo = BasicSoundCreationInfo(file.c_str(),TESTMUSICPATH.c_str(), true);
	soundIdx = audioBackend->createAmbientSound( &basicSoundInfo );
	entity = m_world->createEntity();
	component = new AudioInfo(soundIdx,false);
	entity->addComponent(ComponentType::AudioComponent,component);
	m_world->addEntity(entity);
	
	/************************************************************************/
	/* Load ambient sound													*/
	/************************************************************************/
	file = "Spaceship_Weapon_-_Fighter Blaster or Laser-Shot-Mid.wav";
	basicSoundInfo = BasicSoundCreationInfo(file.c_str(),TESTSOUNDEFFECTPATH.c_str());
	soundIdx = audioBackend->createAmbientSound( &basicSoundInfo );
	entity = m_world->createEntity();
	component = new AudioInfo(soundIdx,false);
	entity->addComponent(ComponentType::AudioComponent,component);
	m_world->addEntity(entity);

	/************************************************************************/
	/* Load ambient sound													*/
	/************************************************************************/
	file = "Spaceship_Engine_Idle_-_Spacecraft_hovering.wav";
	basicSoundInfo = BasicSoundCreationInfo(file.c_str(),TESTSOUNDEFFECTPATH.c_str(),true);
	soundIdx = audioBackend->createAmbientSound( &basicSoundInfo );
	entity = m_world->createEntity();
	component = new AudioInfo(soundIdx,false);
	entity->addComponent(ComponentType::AudioComponent, component);
	m_world->addEntity(entity);
	audioBackend->changeAudioInstruction(soundIdx,SoundEnums::Instructions::PLAY);
}

void ClientApplication::initSoundSystem()
{
	//Audio Systems
	AudioBackendSystem* audioBackend = new AudioBackendSystem();
	m_world->setSystem( SystemType::AudioBackendSystem, audioBackend, true);

	AudioController* audioController = new AudioController(audioBackend);
	m_world->setSystem( SystemType::AudioControllerSystem, audioController, true);

	AudioListenerSystem* audioListener = new AudioListenerSystem(audioBackend);
	m_world->setSystem( SystemType::AudioListenerSystem, audioListener, true);

}

void ClientApplication::InitModulesTestByAnton()
{
	Entity* entity;
	Component* component;

	EntitySystem* tempSys = NULL;

	// Load cube model used as graphic representation for all "graphical" entities.
	tempSys = m_world->getSystem(SystemType::GraphicsBackendSystem);
	GraphicsBackendSystem* graphicsBackend = static_cast<GraphicsBackendSystem*>(tempSys);
	int cubeMeshId = graphicsBackend->createMesh( "P_cube" );
	int shipMeshId = graphicsBackend->createMesh( "Ship.agl", &TESTMODELPATH );
//	int walkerMeshId = graphicsBackend->createMesh( "MeshWalker.agl", &TESTMODELPATH );

	// Create a box that the spaceship can pickup
	entity = m_world->createEntity();
	component = new RenderInfo( cubeMeshId );
	entity->addComponent( ComponentType::RenderInfo, component );
	component = new Transform(10, 0, 0);
	entity->addComponent( ComponentType::Transform, component );

	entity->addComponent( ComponentType::PhysicsBody, 
		new PhysicsBody() );

	entity->addComponent( ComponentType::BodyInitData, 
		new BodyInitData(AglVector3(10, 0, 0),
		AglQuaternion::identity(),
		AglVector3(1, 1, 1), AglVector3(0, 0, 0), 
		AglVector3(0, 0, 0), 0, 
		BodyInitData::DYNAMIC, 
		BodyInitData::SINGLE, false));

	entity->addComponent(ComponentType::ShipModule, new ShipModule());
	entity->addComponent(ComponentType::MinigunModule, new MinigunModule(AglVector3(0, 0, 0), AglVector3(0, 0, 1)));
	m_world->addEntity(entity);

	entity = m_world->createEntity();
	component = new RenderInfo( cubeMeshId );
	entity->addComponent( ComponentType::RenderInfo, component );
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

	m_world->addEntity(entity);

	entity = m_world->createEntity();
	component = new RenderInfo( cubeMeshId );
	entity->addComponent( ComponentType::RenderInfo, component );
	component = new Transform(30, 0, 0);
	entity->addComponent( ComponentType::Transform, component );

	entity->addComponent( ComponentType::PhysicsBody, 
		new PhysicsBody() );

	entity->addComponent( ComponentType::BodyInitData, 
		new BodyInitData(AglVector3(30, 0, 0),
		AglQuaternion::identity(),
		AglVector3(1, 1, 1), AglVector3(0, 0, 0), 
		AglVector3(0, 0, 0), 0, 
		BodyInitData::DYNAMIC, 
		BodyInitData::SINGLE));

	//entity->addComponent(ComponentType::ShipModule, new ShipModule());

	m_world->addEntity(entity);

	entity = m_world->createEntity();
	component = new RenderInfo( cubeMeshId );
	entity->addComponent( ComponentType::RenderInfo, component );
	component = new Transform(40, 0, 0);
	entity->addComponent( ComponentType::Transform, component );

	entity->addComponent( ComponentType::PhysicsBody, 
		new PhysicsBody() );

	entity->addComponent( ComponentType::BodyInitData, 
		new BodyInitData(AglVector3(40, 0, 0),
		AglQuaternion::identity(),
		AglVector3(1, 1, 1), AglVector3(0, 0, 0), 
		AglVector3(0, 0, 0), 0, 
		BodyInitData::DYNAMIC, 
		BodyInitData::SINGLE, false));

	entity->addComponent(ComponentType::ShipModule, new ShipModule());

	m_world->addEntity(entity);

	entity = m_world->createEntity();
	component = new RenderInfo( cubeMeshId );
	entity->addComponent( ComponentType::RenderInfo, component );
	component = new Transform(50, 0, 0);
	entity->addComponent( ComponentType::Transform, component );

	entity->addComponent( ComponentType::PhysicsBody, 
		new PhysicsBody() );

	entity->addComponent( ComponentType::BodyInitData, 
		new BodyInitData(AglVector3(50, 0, 0),
		AglQuaternion::identity(),
		AglVector3(1, 1, 1), AglVector3(0, 0, 0), 
		AglVector3(0, 0, 0), 0, 
		BodyInitData::DYNAMIC, 
		BodyInitData::SINGLE, false));

	entity->addComponent(ComponentType::ShipModule, new ShipModule());
	entity->addComponent(ComponentType::SpeedBoosterModule, new SpeedBoosterModule());

	m_world->addEntity(entity);


	//Ray entity
	/*entity = m_world->createEntity();
	component = new RenderInfo( cubeMeshId );
	entity->addComponent( ComponentType::RenderInfo, component );


	Transform* t = new Transform(AglVector3(0, 0, 0), AglQuaternion::rotateToFrom(AglVector3(0, 0, 1), AglVector3(0, 1, 0)), AglVector3(0.1f, 0.1f, 10));
	entity->addComponent( ComponentType::Transform, t);
	m_world->addEntity(entity);*/
}

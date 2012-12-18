#include "ClientApplication.h"


ClientApplication::ClientApplication( HINSTANCE p_hInstance )
{
	try{
		m_running = false;

		m_hInstance = p_hInstance;

		m_client = new TcpClient();

		m_world = new EntityWorld();
		initSystems();
		initEntities();
		initSounds();
	}
	catch(exception& e)
	{
		DEBUGPRINT((e.what()));
	}
}

ClientApplication::~ClientApplication()
{
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
			
			// DEBUGPRINT(( (toString(dt)+string("\n")).c_str() ));

			m_world->setDelta((float)dt);
			m_world->process();
			
			boost::this_thread::sleep(boost::posix_time::milliseconds(10));
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
	/* Graphics																*/
	/************************************************************************/
	GraphicsBackendSystem* graphicsBackend = new GraphicsBackendSystem( m_hInstance );
	m_world->setSystem( graphicsBackend, true );

	InputBackendSystem* inputBackend = new InputBackendSystem( m_hInstance, graphicsBackend );
	m_world->setSystem( inputBackend, true);

	// Controller system for the ship
	ShipControllerSystem* shipController = new ShipControllerSystem(inputBackend, physics);
	m_world->setSystem( shipController, true);

	// Camera system updates camera based on input and sets its viewport info
	// to the graphics backend for render
	CameraSystem* camera = new CameraSystem( graphicsBackend, inputBackend );
	m_world->setSystem( camera , true );

	RenderPrepSystem* renderer = new RenderPrepSystem( graphicsBackend );
	m_world->setSystem( renderer , true );

	/************************************************************************/
	/* Network																*/
	/************************************************************************/
	ProcessingMessagesSystem* msgProcSystem = new ProcessingMessagesSystem( m_client );
	m_world->setSystem( msgProcSystem , true );


	//Audio Systems
	AudioBackendSystem* audioBackend = new AudioBackendSystem();
	m_world->setSystem( SystemType::AudioBackendSystem, audioBackend, true);

	AudioController* audioController = new AudioController(audioBackend);
	m_world->setSystem( SystemType::AudioControllerSystem, audioController, true);

	AudioListenerSystem* audioListener = new AudioListenerSystem(audioBackend);
	m_world->setSystem( SystemType::AudioListenerSystem, audioListener, true);

	NetworkConnectToServerSystem* connect =
		new NetworkConnectToServerSystem( m_client, inputBackend );
	m_world->setSystem( connect, false );

	NetworkCommunicatorSystem* communicatorSystem =
		new NetworkCommunicatorSystem( m_client );
	m_world->setSystem( communicatorSystem, false );

	
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
				component = new Transform( 2.0f+5.0f*-x, 1.0f+5.0f*-y, 1.0f+5.0f*-z );
				entity->addComponent( ComponentType::Transform, component );

				m_world->addEntity(entity);
			}
		}

	}
	

	//Test physics

	//b1
	entity = m_world->createEntity();
	component = new RenderInfo( cubeMeshId );
	entity->addComponent( ComponentType::RenderInfo, component );
	component = new Transform(AglVector3(0, 0, 0), AglQuaternion(0, 0, 0, 1), AglVector3(1, 1, 1));
	entity->addComponent( ComponentType::Transform, component );
	component = new PhysicsBody();
	entity->addComponent(ComponentType::PhysicsBody, component);

	component = new BodyInitData(AglVector3(0, 0, 0), AglQuaternion::identity(),
									AglVector3(1, 1, 1), AglVector3(1, 0, 0), AglVector3(0, 0, 0), 0, false);
	entity->addComponent(ComponentType::BodyInitData, component);

	m_world->addEntity(entity);

	//b2
	entity = m_world->createEntity();
	component = new RenderInfo( cubeMeshId );
	entity->addComponent( ComponentType::RenderInfo, component );
	component = new Transform(AglVector3(15, 0.5f, 0.5f), AglQuaternion(0, 0, 0, 1), AglVector3(1, 1, 1));
	entity->addComponent( ComponentType::Transform, component );
	component = new PhysicsBody();
	entity->addComponent(ComponentType::PhysicsBody, component);
	
	component = new BodyInitData(AglVector3(15, 0.5f, 0.5f), AglQuaternion::identity(),
		AglVector3(1, 1, 1), AglVector3(-1, 0, 0), AglVector3(0, 0, 0), 0, true);
	entity->addComponent(ComponentType::BodyInitData, component);

	m_world->addEntity(entity);


	// Create a "spaceship"
	entity = m_world->createEntity();
	component = new RenderInfo( cubeMeshId );
	entity->addComponent( ComponentType::RenderInfo, component );
	component = new Transform( -5.0f, 0.0f, 0.0f );
	entity->addComponent( ComponentType::Transform, component );
	component = new ShipController(5.0f, 50.0f);
	entity->addComponent( ComponentType::ShipController, component );
	component = new PhysicsBody();
	entity->addComponent(ComponentType::PhysicsBody, component);

	component = new BodyInitData(AglVector3(-5.0f, 0.0f, 0.0f), AglQuaternion::identity(),
		AglVector3(1, 1, 1), AglVector3(0, 0, 0), AglVector3(0, 0, 0), 0, false);
	entity->addComponent(ComponentType::BodyInitData, component);
	
	m_world->addEntity(entity);
	int shipId = entity->getIndex();



	// A camera from which the world is rendered.
	entity = m_world->createEntity();
	component = new CameraInfo( 800/(float)600 );
	entity->addComponent( ComponentType::CameraInfo, component );
	component = new Input();
	entity->addComponent( ComponentType::Input, component );
	component = new Transform( -5.0f, 0.0f, -5.0f );
	entity->addComponent( ComponentType::Transform, component );
	component = new LookAtEntity(shipId, AglVector3(0,3,-10),10.0f,10.0f);
	entity->addComponent( ComponentType::LookAtEntity, component );
	m_world->addEntity(entity);

	// Misplaced.
	AntTweakBarWrapper::getInstance()->addReadOnlyVariable( "NetId",
		TwType::TW_TYPE_INT32,
		m_client->getIdPointer(), "" );

	// Code below used to test removal of object and components under runtime
	entity = m_world->createEntity();
	component = new Transform( 5.0f, 5.0f, 5.0f );
	entity->addComponent( ComponentType::Transform, component );
	m_world->addEntity(entity);
	m_world->getComponentManager()->removeComponent( entity, ComponentType::Transform );
	m_world->deleteEntity(entity);

}

void ClientApplication::initSounds()
{
	EntitySystem*	tempSys			= NULL;
	Entity*			entity			= NULL;
	Component*		component		= NULL;
	BasicSoundCreationInfo* basicSoundInfo	= NULL;
	PositionalSoundCreationInfo* positionalSoundInfo = NULL;
	int				soundIdx		= -1;
	string			fullFilePath;
	string			file;

	tempSys = m_world->getSystem(SystemType::AudioBackendSystem);
	AudioBackendSystem* audioBackend = static_cast<AudioBackendSystem*>(tempSys);


	/************************************************************************/
	/* Load positional sound												*/
	/************************************************************************/

	file = "MusicMono.wav";
	fullFilePath = TESTMUSICPATH + file;

	basicSoundInfo = new BasicSoundCreationInfo(file.c_str(),fullFilePath.c_str(),true);
	positionalSoundInfo = new PositionalSoundCreationInfo(AglVector3(3,3,3));
	soundIdx = audioBackend->createPositionalSound(basicSoundInfo,positionalSoundInfo);
	entity = m_world->createEntity();
	component = new Transform( 3.0f, -10.0f, -30.0f );
	entity->addComponent( ComponentType::Transform, component );
	component = new AudioInfo(soundIdx,true);
	entity->addComponent(ComponentType::AudioComponent, component);
	m_world->addEntity(entity);
	audioBackend->changeAudioInstruction(soundIdx, SoundEnums::Instructions::PLAY);

	delete basicSoundInfo;
	delete positionalSoundInfo;

	/************************************************************************/
	/* Load positional sound												*/
	/************************************************************************/
	
	file = "MusicMono.wav";
	fullFilePath = TESTMUSICPATH+file;

	basicSoundInfo = new BasicSoundCreationInfo(file.c_str(),fullFilePath.c_str(),true);
	positionalSoundInfo = new PositionalSoundCreationInfo(AglVector3(3,3,3));
	soundIdx = audioBackend->createPositionalSound(basicSoundInfo,positionalSoundInfo);
	entity = m_world->createEntity();
	component = new Transform( 3.0f, 3.0f, 3.0f );
	entity->addComponent( ComponentType::Transform, component );
	component = new AudioInfo(soundIdx,true);
	entity->addComponent(ComponentType::AudioComponent, component);
	m_world->addEntity(entity);
	audioBackend->changeAudioInstruction(soundIdx, SoundEnums::Instructions::PLAY);

	delete basicSoundInfo;
	delete positionalSoundInfo;

	/************************************************************************/
	/* Load ambient sound													*/
	/************************************************************************/
	file = "Techno_1.wav";
	fullFilePath = TESTMUSICPATH+file;

	basicSoundInfo = new BasicSoundCreationInfo(file.c_str(),fullFilePath.c_str(), true);
	soundIdx = audioBackend->createAmbientSound( basicSoundInfo );
	entity = m_world->createEntity();
	component = new AudioInfo(soundIdx,false);
	entity->addComponent(ComponentType::AudioComponent,component);
	m_world->addEntity(entity);

	delete basicSoundInfo;
	
	/************************************************************************/
	/* Load ambient sound													*/
	/************************************************************************/
	file = "spaceship_laser.wav";
	fullFilePath = TESTSOUNDEFFECTPATH+file;
	
	basicSoundInfo = new BasicSoundCreationInfo(file.c_str(),fullFilePath.c_str(), false);
	soundIdx = audioBackend->createAmbientSound( basicSoundInfo );
	entity = m_world->createEntity();
	component = new AudioInfo(soundIdx,false);
	entity->addComponent(ComponentType::AudioComponent,component);
	m_world->addEntity(entity);

	delete basicSoundInfo;
	
}

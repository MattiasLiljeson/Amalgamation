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
			currTimeStamp = 0;
			QueryPerformanceCounter((LARGE_INTEGER*)&currTimeStamp);
			dt = (currTimeStamp - m_prevTimeStamp) * secsPerCount;
			dt = 1/100.0;
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

	// Input depends on callback loop in the graphicsBackend. No mouse/keyboard inputs
	// will be available if the backend systems isn't used. 
	InputSystem* input = new InputSystem();
	m_world->setSystem( SystemType::InputSystem, input, true);

	// Physics systems
	PhysicsSystem* physics = new PhysicsSystem();
	m_world->setSystem(SystemType::PhysicsSystem, physics, true);
	
	// Graphic systems
	GraphicsBackendSystem* graphicsBackend = new GraphicsBackendSystem( m_hInstance );
	m_world->setSystem( SystemType::GraphicsBackendSystem, graphicsBackend, true );

	CameraSystem* camera = new CameraSystem( graphicsBackend );
	m_world->setSystem( SystemType::CameraSystem, camera , true );

	RenderPrepSystem* renderer = new RenderPrepSystem( graphicsBackend );
	m_world->setSystem( SystemType::RenderPrepSystem, renderer , true );

	// Network systems
	NetworkConnectToServerSystem* connect = new NetworkConnectToServerSystem( m_client );
	m_world->setSystem( SystemType::NetworkConnectoToServerSystem, connect, false );


	//Audio Systems
	AudioBackendSystem* audioBackend = new AudioBackendSystem();
	m_world->setSystem( SystemType::AudioBackendSystem, audioBackend, true);

	AudioController* audioController = new AudioController(audioBackend);
	m_world->setSystem( SystemType::AudioControllerSystem, audioController, true);

	AudioListenerSystem* audioListener = new AudioListenerSystem(audioBackend);
	m_world->setSystem( SystemType::AudioListenerSystem, audioListener, true);

	m_world->initialize();
}

void ClientApplication::initEntities()
{
	Entity* entity;
	Component* component;

	// Physics object without a model defined, will not be rendered.
	entity = m_world->createEntity();
	component = new RenderInfo();
	entity->addComponent( ComponentType::RenderInfo, component );
	component = new Transform();
	entity->addComponent( ComponentType::Transform, component );
	component = new PhysicsBody();
	entity->addComponent(ComponentType::PhysicsBody, component);
	m_world->addEntity(entity);

	EntitySystem* tempSys = NULL;

	// Load cube model used as graphic representation for all "graphical" entities.
	tempSys = m_world->getSystem(SystemType::GraphicsBackendSystem);
	GraphicsBackendSystem* graphicsBackend = static_cast<GraphicsBackendSystem*>(tempSys);
	int cubeMeshId = graphicsBackend->getMeshId( "P_cube" );




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

	// A camera from which the world is rendered.
	entity = m_world->createEntity();
	component = new CameraInfo( 800/(float)600 );
	entity->addComponent( ComponentType::CameraInfo, component );
	component = new Input();
	entity->addComponent( ComponentType::Input, component );
	component = new Transform( 5.0f, 5.0f, 5.0f );
	entity->addComponent( ComponentType::Transform, component );
	m_world->addEntity(entity);

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
	BasicSoundCreationInfo* info	= NULL;
	int				soundIdx		= -1;
	string			fullFilePath;

	tempSys = m_world->getSystem(SystemType::AudioBackendSystem);
	AudioBackendSystem* audioBackend = static_cast<AudioBackendSystem*>(tempSys);

	/************************************************************************/
	/* Load positional sound												*/
	/************************************************************************/
	
	fullFilePath = MUSICPATH;
	fullFilePath += "Test/MusicMono.wav";

	info = new BasicSoundCreationInfo("Assets/Sound/Music/Test/MusicMono.wav",true);
	soundIdx = audioBackend->createPositionalSound(info,AglVector3( 3.0f, 3.0f, 3.0f ));
	entity = m_world->createEntity();
	component = new Transform( 3.0f, 3.0f, 3.0f );
	entity->addComponent( ComponentType::Transform, component );
	component = new AudioInfo(soundIdx,true);
	entity->addComponent(ComponentType::AudioComponent, component);
	m_world->addEntity(entity);
	audioBackend->changeAudioInstruction(soundIdx, SoundEnums::Instructions::STOP);

	delete info;
	
	/************************************************************************/
	/* Load positional sound												*/
	/************************************************************************/
	
	fullFilePath = MUSICPATH;
	fullFilePath += "Test/MusicMono.wav";

	info = new BasicSoundCreationInfo("Assets/Sound/Music/Test/MusicMono.wav",true);
	soundIdx = audioBackend->createPositionalSound(info,AglVector3( 3.0f, -10.0f, 3.0f ));
	entity = m_world->createEntity();
	component = new Transform( 3.0f, -10.0f, 3.0f );
	entity->addComponent( ComponentType::Transform, component );
	component = new AudioInfo(soundIdx,true);
	entity->addComponent(ComponentType::AudioComponent, component);
	m_world->addEntity(entity);
	audioBackend->changeAudioInstruction(soundIdx, SoundEnums::Instructions::STOP);

	delete info;
	
	/************************************************************************/
	/* Load ambient sound													*/
	/************************************************************************/

	fullFilePath = MUSICPATH;
	fullFilePath += "Test/Techno_1.wav";

	info = new BasicSoundCreationInfo(fullFilePath.c_str(), true);
	soundIdx = audioBackend->createAmbientSound( info );
	entity = m_world->createEntity();
	component = new AudioInfo(soundIdx,false);
	entity->addComponent(ComponentType::AudioComponent,component);
	m_world->addEntity(entity);
	audioBackend->changeAudioInstruction(soundIdx, SoundEnums::STOP);

	delete info;
	
	/************************************************************************/
	/* Load ambient sound													*/
	/************************************************************************/
	fullFilePath = SOUNDEFFECTPATH;
	fullFilePath += "Test/spaceship_laser.wav";
	info = new BasicSoundCreationInfo(fullFilePath.c_str(), false);
	soundIdx = audioBackend->createAmbientSound( info );
	entity = m_world->createEntity();
	component = new AudioInfo(soundIdx,false);
	entity->addComponent(ComponentType::AudioComponent,component);
	m_world->addEntity(entity);

	delete info;
}

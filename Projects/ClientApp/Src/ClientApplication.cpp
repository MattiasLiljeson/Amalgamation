#include "ClientApplication.h"


ClientApplication::ClientApplication( HINSTANCE p_hInstance )
{
	m_running = false;

	m_hInstance = p_hInstance;
	
	m_client = new TcpClient();

	m_world = new EntityWorld();
	initSystems();
	initEntities();
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
	// will be available if the graphics backend system isn't used. 
	InputBackendSystem* inputBackend = new InputBackendSystem();
	m_world->setSystem( SystemType::InputBackendSystem, inputBackend, true);

	// Physics systems
	PhysicsSystem* physics = new PhysicsSystem();
	m_world->setSystem(SystemType::PhysicsSystem, physics, true);
	
	// Graphic systems
	GraphicsBackendSystem* graphicsBackend = new GraphicsBackendSystem( m_hInstance );
	m_world->setSystem( SystemType::GraphicsBackendSystem, graphicsBackend, true );

	// Controller system for the ship
	ShipControllerSystem* shipController = new ShipControllerSystem(inputBackend);
	m_world->setSystem(SystemType::ShipControllerSystem, shipController, true);

	// Camera system updates camera based on input and sets its viewport info
	// to the graphics backend for render
	CameraSystem* camera = new CameraSystem( graphicsBackend, inputBackend );
	m_world->setSystem( SystemType::CameraSystem, camera , true );

	RenderPrepSystem* renderer = new RenderPrepSystem( graphicsBackend );
	m_world->setSystem( SystemType::RenderPrepSystem, renderer , true );

	// Network systems
	ProcessingMessagesSystem* msgProcSystem = new ProcessingMessagesSystem( m_client );
	m_world->setSystem( SystemType::ProcessingMessagesSystem, msgProcSystem , true );

	NetworkConnectToServerSystem* connect =
		new NetworkConnectToServerSystem( m_client );
	m_world->setSystem( SystemType::NetworkConnectoToServerSystem, connect,
		false );

	NetworkCommunicatorSystem* communicatorSystem =
		new NetworkCommunicatorSystem( m_client );
	m_world->setSystem( SystemType::NetworkCommunicatorSystem, communicatorSystem,
		false );

	
	m_world->initialize();
}

void ClientApplication::initEntities()
{
	Entity* entity;
	Component* component;

	// Load cube model used as graphic representation for all "graphical" entities.
	EntitySystem* sys = m_world->getSystem(SystemType::GraphicsBackendSystem);
	GraphicsBackendSystem* graphicsBackend = static_cast<GraphicsBackendSystem*>(sys);
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
	component = new Transform( 0.0f, 0.0f, 0.0f );
	entity->addComponent( ComponentType::Transform, component );
	component = new ShipController(2.0f,10.0f);
	entity->addComponent( ComponentType::ShipController, component );
	m_world->addEntity(entity);


	// A camera from which the world is rendered.
	entity = m_world->createEntity();
	component = new CameraInfo( 800/(float)600 );
	entity->addComponent( ComponentType::CameraInfo, component );
	component = new Input();
	entity->addComponent( ComponentType::Input, component );
	component = new Transform( 5.0f, 5.0f, 5.0f );
	entity->addComponent( ComponentType::Transform, component );
	m_world->addEntity(entity);

	// Code below used to test removal of object and compoennts under runtime
	entity = m_world->createEntity();
	component = new Transform( 5.0f, 5.0f, 5.0f );
	entity->addComponent( ComponentType::Transform, component );
	m_world->addEntity(entity);
	m_world->getComponentManager()->removeComponent( entity, ComponentType::Transform );
	m_world->deleteEntity(entity);
}


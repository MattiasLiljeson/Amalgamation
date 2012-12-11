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
	__int64 cntsPerSec = 0;
	__int64 currTimeStamp = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&cntsPerSec);
	double secsPerCnt = 1.0f / (float)cntsPerSec;

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
			dt = (currTimeStamp - m_prevTimeStamp) * secsPerCnt;
			dt = 1/100.0;
			m_prevTimeStamp = currTimeStamp;

			m_world->setDelta(dt);
			m_world->process();
		}
	}

}

void ClientApplication::initSystems()
{
	GraphicsBackendSystem* gfxSys = new GraphicsBackendSystem( m_hInstance );
	m_world->setSystem( SystemType::SystemTypeIdx::GraphicsBackendSystem, gfxSys , true );

	CameraSystem* camSys = new CameraSystem( gfxSys );
	m_world->setSystem( SystemType::SystemTypeIdx::CameraSystem, camSys , true );

	RenderPrepSystem* rpSys = new RenderPrepSystem( gfxSys );
	m_world->setSystem( SystemType::RenderPrepSystem, rpSys , true );

	InputSystem* inSys = new InputSystem();
	m_world->setSystem( SystemType::RenderPrepSystem, inSys, true);

	m_world->initialize();
}

void ClientApplication::initEntities()
{
	Entity* e = m_world->createEntity();
	Component* c = new RenderInfo();
	e->addComponent( ComponentType::RenderInfo, c );
	m_world->addEntity(e);

	e = m_world->createEntity();
	c = new CameraInfo(800/(float)600);
	e->addComponent( ComponentType::CameraInfo, c );
	c = new Input();
	e->addComponent( ComponentType::Input, c );
	m_world->addEntity(e);
}

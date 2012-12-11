
#include <CameraSystem.h>
#include <GraphicsBackendSystem.h>
#include <RenderPrepSystem.h>
#include <EntityWorld.h>
#include <Input.h>
#include <InputSystem.h>
#include <RenderInfo.h>
#include <Windows.h>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	EntityWorld* world;
	world = new EntityWorld();

	GraphicsBackendSystem* gfxSys = new GraphicsBackendSystem( hInstance );
	world->setSystem( SystemType::SystemTypeIdx::GraphicsBackendSystem, gfxSys , true );

	CameraSystem* camSys = new CameraSystem( gfxSys );
	world->setSystem( SystemType::SystemTypeIdx::CameraSystem, camSys , true );

	RenderPrepSystem* rpSys = new RenderPrepSystem( gfxSys );
	world->setSystem( SystemType::RenderPrepSystem, rpSys , true );

	InputSystem* inSys = new InputSystem();
	world->setSystem( SystemType::RenderPrepSystem, inSys, true);

	Entity* e = world->createEntity();
	Component* c = new RenderInfo();
	e->addComponent( ComponentType::RenderInfo, c );
	world->addEntity(e);

	e = world->createEntity();
	c = new CameraInfo(800/(float)600);
	e->addComponent( ComponentType::CameraInfo, c );
	c = new Input();
	e->addComponent( ComponentType::Input, c );

	world->addEntity(e);

	world->initialize();

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

			world->setDelta(dt);
			world->process();
		}
	}

	delete world;
	return 0;
}
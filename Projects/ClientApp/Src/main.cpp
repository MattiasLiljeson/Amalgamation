#include "CameraSystem.h"
#include "GraphicsBackendSystem.h"

#include <Windows.h>
#include <EntityWorld.h>
#include "RenderPrepSystem.h"
#include <RenderInfo.h>

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


	Entity* e = world->createEntity();
	Component* c = new RenderInfo();
	e->addComponent( ComponentType::RenderInfo, c );
	world->addEntity(e);

	e = world->createEntity();
	c = new CameraInfo(800/(float)600);
	e->addComponent( ComponentType::CameraInfo, c );
	world->addEntity(e);

	world->initialize();

	// simple timer
	__int64 cntsPerSec = 0;
	__int64 currTimeStamp = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&cntsPerSec);
	double secsPerCnt = 1.0f / (float)cntsPerSec;

	double dt = 0.0f;
	__int64 m_prevTimeStamp = 0;
	while( true )
	{
		currTimeStamp = 0;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTimeStamp);
		dt = (currTimeStamp - m_prevTimeStamp) * secsPerCnt;
		dt = 1/100.0;
		m_prevTimeStamp = currTimeStamp;

		world->setDelta(dt);
		world->process();
	}

	delete world;
	return 0;
}
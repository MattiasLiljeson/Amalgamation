#include <Windows.h>
#include <EntityWorld.h>
#include "RenderPrepSystem.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	EntityWorld* world;
	world = new EntityWorld();

	RenderPrepSystem* rpSys = new RenderPrepSystem( hInstance );

	world->setSystem(SystemType::RenderPrepSystem, rpSys , true);

	float dt = 1/60.0f;
	while( true )
	{
		world->setDelta(dt);
		world->process();
	}

	delete world;
	return 0;
}
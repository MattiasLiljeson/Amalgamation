#ifdef DEBUG
#include "vld.h"
#endif

#include <Windows.h>
#include <iostream>
#include "Window.h"
#include "D3DRender.h"
#include "DebugUtil.h"

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, 
	int nCmdShow )
{
	Window* window;
	D3DRender* renderer;
	try
	{
		window = new Window(hInstance,800,600,1);
		renderer = new D3DRender(window->getWindowRef(),800,600,true);
	}
	catch (exception &e)
	{
		DEBUGPRINT((e.what()));
	}

	__int64 cntsPerSec = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&cntsPerSec);
	float secsPerCnt = 1.0f / (float)cntsPerSec;

	__int64 prevTimeStamp = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&prevTimeStamp);

	// Main message loop
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
			__int64 currTimeStamp = 0;
			QueryPerformanceCounter((LARGE_INTEGER*)&currTimeStamp);
			float dt = (currTimeStamp - prevTimeStamp) * secsPerCnt;

			prevTimeStamp = currTimeStamp;

			renderer->beginDraw();
			renderer->presentFrame();
		}
	}

	delete window;
	delete renderer;

	return 0;
}
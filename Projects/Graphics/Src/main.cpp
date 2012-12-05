#include <Windows.h>
#include <iostream>
#include <vld.h>
#include "TextureParser.h"
#include "AntTweakBarWrapper.h"
#include "Window.h"
#include "D3DRender.h"
#include "DebugUtil.h"

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, 
	int nCmdShow )
{
	Window* window;
	D3DRender* renderer;

	TextureParser::init();
	
	try
	{
		window = new Window(hInstance,800,600,1);
		renderer = new D3DRender(window->getWindowRef(),800,600,true);
		AntTweakBarWrapper::getInstance(renderer->getDevice(),"Drunken Bar");
		renderer->hookUpAntTweakBar();
	}
	catch (exception &e)
	{
		DEBUGPRINT((e.what()));
		return -1;
	}
	
	__int64 cntsPerSec = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&cntsPerSec);
	float secsPerCnt = 1.0f / (float)cntsPerSec;

	__int64 prevTimeStamp = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&prevTimeStamp);

	RendererMeshInfo testMeshInfo = {{0.0f,0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f,0.0f}};

	// Temporary static view projection matrix
	// column major. 
	// R=row, col=column, r=rotation, s=scale, t=translation
	RendererSceneInfo tempSceneInfo = 
	{
	  // R0rs    R1rs   R2rs     R3t
		{1.0f,   0.0f,  0.0f,    0.0f,  // col 0  x
		 0.0f,   1.0f,  0.0f,    0.0f,  // col 1  y
		 0.0f,   0.0f,  1.0f,    0.0f,  // col 2  z
		 0.0f,   0.0f,  0.0f,    1.0f}  // col 3  w
	};


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

			// * Scene render preparation system * 1
			renderer->setSceneInfo(tempSceneInfo);// sets up certain "global" scene data 
					
			// * Deferred base system *            1
			renderer->clearRenderTargets();	      // clear render targets used           
			renderer->beginFrame();				  // prepare frame, set drawing to MRT   

			// * Render system *                   N
			renderer->renderMesh(testMeshInfo);	  // process a mesh						 

			// * Deferred finalize system *        1
			renderer->finalizeFrame();			  // finalize, draw to backbuffer        
			AntTweakBarWrapper::getInstance()->render();
			renderer->flipBackBuffer();           // flip buffers						 
		}
	}

	AntTweakBarWrapper::destroy();
	delete window;
	delete renderer;

	return 0;
}
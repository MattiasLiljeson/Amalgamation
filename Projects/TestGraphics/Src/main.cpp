#include <vld.h>
#include <Windows.h>
#include <iostream>
#include "TextureParser.h"
#include "AntTweakBarWrapper.h"
#include "Window.h"
#include "GraphicsWrapper.h"
#include <DebugUtil.h>

// temporary usage of these in main for mesh creation
#include "Mesh.h"

// math tmp
#include <AglMatrix.h>
#include <AglVector3.h>

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, 
					int nCmdShow )
{
	Window* window;
	GraphicsWrapper* graphicsWrapper;

	SetThreadAffinityMask(GetCurrentThread(), 1);

	TextureParser::init();

	try
	{
		window = new Window(hInstance,800,600,1);
		graphicsWrapper = new GraphicsWrapper(window->getWindowRef(),800,600,true);
		AntTweakBarWrapper::getInstance(graphicsWrapper->getDevice(),"Drunken Bar");
		graphicsWrapper->hookUpAntTweakBar();
	}
	catch (exception &e)
	{
		DEBUGWARNING((e.what()));
		return -1;
	}

	__int64 cntsPerSec = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&cntsPerSec);
	float secsPerCnt = 1.0f / (float)cntsPerSec;

	__int64 prevTimeStamp = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&prevTimeStamp);

	// Create a cube
	unsigned int cubeId = graphicsWrapper->createMeshFromRaw("P_cube");

	// Create a shitload of instances
	vector<InstanceData>* instances = new vector<InstanceData>();
	for (int i=0;i<100;i++)
	{

		{
			InstanceData t={1.0f,0.0f,0.0f,  0.0f,
				0.0f,1.0f,0.0f,  0.0f,
				0.0f,0.0f,1.0f,  0.0f,
				0.0f,0.0f,0.0f,  1.0f};
			instances->push_back(t);
		}
	
		{
			InstanceData t={1.0f,0.0f,0.0f, -3.0f-(float)i*2.3f,
				0.0f,1.0f,0.0f,  0.0f,
				0.0f,0.0f,1.0f,  2.0f,
				0.0f,0.0f,0.0f,  1.0f};
			instances->push_back(t);
		}

		{
			InstanceData t={1.0f,0.0f,0.0f, 3.0f+(float)i*2.3f,
			0.0f,1.0f,0.0f,  (float)i*2.3f+2.0f,
			0.0f,0.0f,1.0f,  0.0f,
			0.0f,0.0f,0.0f,  1.0f};
			instances->push_back(t);
		}

		{
			InstanceData t={1.0f,0.0f,0.0f,  0.0f,
			0.0f,1.0f,0.0f,  0.0f,
			0.0f,0.0f,1.0f,  (float)i*2.3f+2.0f,
			0.0f,0.0f,0.0f,  1.0f};
			instances->push_back(t);
		}

		{
			InstanceData t={1.0f,0.0f,0.0f,  3.0f+(float)i*2.3f,
				0.0f,1.0f,0.0f,  (float)i*2.3f+2.0f,
			0.0f,0.0f,1.0f,  -(float)i*2.3f-2.0f,
			0.0f,0.0f,0.0f,  1.0f};
			instances->push_back(t);
		}

		{
			InstanceData t={1.0f,0.0f,0.0f,  0.0f,
			0.0f,1.0f,0.0f,  (float)i*2.3f+2.0f,
			0.0f,0.0f,1.0f,  -2.0f,
			0.0f,0.0f,0.0f,  1.0f};
			instances->push_back(t);
		}
	}


	AglMatrix viewMatrix = AglMatrix::identityMatrix();
	AglMatrix projMatrix = AglMatrix::identityMatrix();
	AglMatrix camMatrix = AglMatrix::identityMatrix();
	float ticker = 0.0f;

	AglVector3 pos(0.0f,0.0f,-3.0f);
	AglVector3 lookAt(0.0f,0.0f,0.0f);
	AglVector3 up(0.0f,1.0f,0.0f);

	viewMatrix = AglMatrix::createViewMatrix(pos, lookAt, up);
	projMatrix = AglMatrix::createPerspectiveMatrix(3.14f/2.0f,800.0f/600.0f,0.1f,100.0f);

	camMatrix = AglMatrix::transpose(AglMatrix::identityMatrix()*viewMatrix*projMatrix);

	RendererSceneInfo tempSceneInfo;
	for (int n=0;n<16;n++)
		tempSceneInfo.viewProjectionMatrix[n] = camMatrix[n];

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

			// temp camera test
			{
				ticker += dt;
				pos.z = -2.0f-sin(ticker);
				pos.x = sin(ticker);
				pos.y = -2.0f-cos(ticker);

				viewMatrix = AglMatrix::createViewMatrix(pos, lookAt, up);

				camMatrix = AglMatrix::transpose(AglMatrix::identityMatrix()*viewMatrix*projMatrix);

				for (int n=0;n<16;n++)
					tempSceneInfo.viewProjectionMatrix[n] = camMatrix[n];
			}

			// * Scene render preparation system * 1
			graphicsWrapper->setSceneInfo(tempSceneInfo);// sets up certain "global" scene data 

			// * Deferred base system *            1
			graphicsWrapper->clearRenderTargets();	      // clear render targets used           
			graphicsWrapper->beginFrame();				  // prepare frame, set drawing to MRT   

			// * Render system *                   N
			graphicsWrapper->renderMesh(cubeId,instances);	  // process a mesh						 

			// * Deferred finalize system *        1
			graphicsWrapper->finalizeFrame();			  // finalize, draw to backbuffer        
			AntTweakBarWrapper::getInstance()->render();
			graphicsWrapper->flipBackBuffer();           // flip buffers						 
		}
	}

	AntTweakBarWrapper::destroy();
	delete instances;
	delete window;
	delete graphicsWrapper;

	return 0;
}
#include "RenderPrepSystem.h"

#include <CamMatrixerUtil.h>

RenderPrepSystem::RenderPrepSystem( HINSTANCE p_hInstance, int p_scrWidth, int p_scrHeight,
								   bool p_windowed  )
								   : EntitySystem( SystemType::RenderPrepSystem )
{
	m_hInstance = p_hInstance;
	m_scrWidth = p_scrWidth;
	m_scrHeight = p_scrHeight;
}

RenderPrepSystem::~RenderPrepSystem()
{
	delete m_window;
	delete m_graphicsWrapper;
	AntTweakBarWrapper::destroy();
}

void RenderPrepSystem::initialize()
{
	TextureParser::init();

	try
	{
		m_window = new Window( m_hInstance, m_scrWidth, m_scrHeight, 1);
		m_graphicsWrapper = new GraphicsWrapper( m_window->getWindowRef(), m_scrWidth, m_scrHeight, true );
		AntTweakBarWrapper::getInstance( m_graphicsWrapper->getDevice(), "Drunken Bar" );
		m_graphicsWrapper->hookUpAntTweakBar();
	}
	catch( exception &e )
	{
		DEBUGPRINT( (e.what()) );
	}

	m_cntsPerSec = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_cntsPerSec);
	m_secsPerCnt = 1.0f / (float)m_cntsPerSec;
	m_prevTimeStamp = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&m_prevTimeStamp);

	// Create a cube
	unsigned int m_cubeId = m_graphicsWrapper->createMesh( "P_cube", 0 );
	// RendererMeshInfo testMeshInfo = {{0.0f,0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f,0.0f},m_cubeId};

	m_viewMatrix = AglMatrix::identityMatrix();
	m_projMatrix = AglMatrix::identityMatrix();
	m_camMatrix = AglMatrix::identityMatrix();
	float ticker = 0.0f;

	m_pos = AglVector3( 0.0f, 0.0f, -3.0f );
	m_lookAt = AglVector3( 0.0f, 0.0f, 0.0f );
	m_up = AglVector3( 0.0f, 1.0f, 0.0f );
	m_ticker = 0.0f;

	SetLookAtMatrix( m_viewMatrix, m_pos, m_lookAt, m_up );
	float aspectRatio = (float)m_scrWidth / (float)m_scrHeight;
	float fovAsRadians = 3.14f/2.0f;
	float nearClip = 0.1f;
	float farClip = 100.0f;
	SetProjMatrix( m_projMatrix, fovAsRadians, aspectRatio, nearClip, farClip );

	m_camMatrix = AglMatrix::transpose( AglMatrix::identityMatrix() * m_viewMatrix * m_projMatrix );

	for( int n=0; n<16; n++ )
		m_sceneInfo.viewProjectionMatrix[n] = m_camMatrix[n];
}

void RenderPrepSystem::process()
{
	MSG msg = {0};
	if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE) )
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
	else
	{
		__int64 currTimeStamp = 0;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTimeStamp);
		float dt = (currTimeStamp - m_prevTimeStamp) * m_secsPerCnt;

		m_prevTimeStamp = currTimeStamp;

		// temp camera test
		{
			m_ticker += dt;
			m_pos.z = -1.0f-sin(m_ticker);
			m_pos.x = -1.0f-sin(m_ticker);
			m_pos.y = -1.0f-cos(m_ticker);

			SetLookAtMatrix(m_viewMatrix, m_pos, m_lookAt, m_up);

			m_camMatrix = AglMatrix::transpose( AglMatrix::identityMatrix() * m_viewMatrix * m_projMatrix );

			for( int n=0; n<16; n++ )
				m_sceneInfo.viewProjectionMatrix[n] = m_camMatrix[n];
		}

		// * Scene render preparation system * 1
		m_graphicsWrapper->setSceneInfo(m_sceneInfo);// sets up certain "global" scene data 

		// * Deferred base system *            1
		m_graphicsWrapper->clearRenderTargets();	      // clear render targets used           
		m_graphicsWrapper->beginFrame();				  // prepare frame, set drawing to MRT   

		// * Render system *                   N
		m_graphicsWrapper->renderMesh(m_cubeId);	  // process a mesh						 

		// * Deferred finalize system *        1
		m_graphicsWrapper->finalizeFrame();			  // finalize, draw to backbuffer        
		AntTweakBarWrapper::getInstance()->render();
		m_graphicsWrapper->flipBackBuffer();           // flip buffers	
	}
}

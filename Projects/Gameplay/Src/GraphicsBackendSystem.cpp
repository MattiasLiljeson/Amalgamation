#include "GraphicsBackendSystem.h"

GraphicsBackendSystem::GraphicsBackendSystem( HINSTANCE p_hInstance, int p_scrWidth, int p_scrHeight,
											 bool p_windowed  ) : EntitySystem( SystemType::RenderPrepSystem )
{
	m_hInstance = p_hInstance;
	m_scrWidth = p_scrWidth;
	m_scrHeight = p_scrHeight;
}


GraphicsBackendSystem::~GraphicsBackendSystem(void)
{
	delete m_window;
	delete m_graphicsWrapper;
	AntTweakBarWrapper::destroy();
}

void GraphicsBackendSystem::initialize()
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
}

void GraphicsBackendSystem::process()
{

}

int GraphicsBackendSystem::getMeshId( string p_meshName )
{
	return m_graphicsWrapper->createMesh( p_meshName );
}

GraphicsWrapper* GraphicsBackendSystem::getGfxWrapper()
{
	return m_graphicsWrapper;
}

HWND GraphicsBackendSystem::getWindowRef()
{
	return m_window->getWindowRef();
}

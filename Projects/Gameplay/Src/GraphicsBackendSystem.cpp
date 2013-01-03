#include "GraphicsBackendSystem.h"
#include <AntTweakBarWrapper.h>
#include <TextureParser.h>
#include <DebugUtil.h>
#include <EntitySystem.h>
#include <GraphicsWrapper.h>
#include <Window.h>
#include <SystemType.h>
#include <ComponentType.h>

GraphicsBackendSystem::GraphicsBackendSystem( HINSTANCE p_hInstance, 
											 int p_scrWidth /* = 800 */, 
											 int p_scrHeight /* = 600 */, 
											 bool p_windowed /* = true */)
											 : EntitySystem( 
											 SystemType::GraphicsBackendSystem )
											 
{
	m_hInstance = p_hInstance;
	m_scrWidth = p_scrWidth;
	m_scrHeight = p_scrHeight;
	m_windowed = p_windowed;
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
		m_graphicsWrapper = new GraphicsWrapper( m_window->getWindowRef(), 
			m_scrWidth, 
			m_scrHeight, 
			m_windowed );
		AntTweakBarWrapper::getInstance( m_graphicsWrapper->getDevice(), "Drunken_Bar" );
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

unsigned int GraphicsBackendSystem::createMesh( const string& p_meshName, 
											    const string* p_path/*=NULL */ )
{
	return m_graphicsWrapper->createMesh(p_meshName,p_path);
}

int GraphicsBackendSystem::getMeshId( const string& p_meshName )
{
	return m_graphicsWrapper->getMeshId(p_meshName);
}

GraphicsWrapper* GraphicsBackendSystem::getGfxWrapper()
{
	return m_graphicsWrapper;
}

HWND GraphicsBackendSystem::getWindowRef()
{
	return m_window->getWindowRef();
}

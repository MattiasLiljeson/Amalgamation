#include "GraphicsBackendSystem.h"
#include <TextureParser.h>
#include <DebugUtil.h>
#include <EntitySystem.h>
#include <GraphicsWrapper.h>
#include <Window.h>
#include <SystemType.h>
#include <ComponentType.h>
#include "CameraInfo.h"

GraphicsBackendSystem* GraphicsBackendSystem::m_selfPointer = NULL;

GraphicsBackendSystem::GraphicsBackendSystem( HINSTANCE p_hInstance, 
											 int p_scrWidth /* = 800 */, 
											 int p_scrHeight /* = 600 */, 
											 bool p_windowed /* = true */)
											 : EntitySystem( 
											 SystemType::GraphicsBackendSystem )
											 
{
	m_hInstance = p_hInstance;
	m_newWidth = m_scrWidth = p_scrWidth;
	m_newHeight = m_scrHeight = p_scrHeight;
	m_windowed = p_windowed;

	/************************************************************************/
	/* ONLY NEEDED OF THE ANTTWEAKBAR CALLBACK								*/
	/************************************************************************/
	m_selfPointer = this;
}


GraphicsBackendSystem::~GraphicsBackendSystem(void)
{
	delete m_window;
	delete m_graphicsWrapper;
	AntTweakBarWrapper::destroy();
}

void GraphicsBackendSystem::changeResolution( int p_scrWidth, int p_scrHeight )
{
	m_scrWidth = p_scrWidth;
	m_scrHeight = p_scrHeight;

	// Resize the actual window.
	m_window->changeWindowRes( p_scrWidth, p_scrHeight );

	// Resize the back buffer.
	m_graphicsWrapper->changeBackbufferRes( p_scrWidth, p_scrHeight );
}

void GraphicsBackendSystem::initialize()
{
	TextureParser::init();

	m_window = new Window( m_hInstance, m_scrWidth, m_scrHeight, 1);
	m_graphicsWrapper = new GraphicsWrapper( m_window->getWindowRef(), 
		m_scrWidth, 
		m_scrHeight, 
		m_windowed );

	AntTweakBarWrapper::getInstance( m_graphicsWrapper->getDevice(), "Drunken_Bar" );

	TwAddButton(AntTweakBarWrapper::getInstance()->getMainBar(),
		"Toggle_Windowed/FullScreen",
		toggleFullScreen, (void*)NULL, "");

	m_graphicsWrapper->hookUpAntTweakBar();

	// Anttweakbar resolution
	AntTweakBarWrapper::getInstance()->addWriteVariable( "Win width",
		TwType::TW_TYPE_INT32, &m_newWidth, "min=800 max=4096" );
	
	AntTweakBarWrapper::getInstance()->addWriteVariable( "Win height",
		TwType::TW_TYPE_INT32, &m_newHeight, "min=480 max=4096" );

	TwAddButton(AntTweakBarWrapper::getInstance()->getMainBar(),
		"Apply resolution",
		applyNewResolution, (void*)NULL, "");
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

void TW_CALL GraphicsBackendSystem::toggleFullScreen(void* p_clientData)
{
	m_selfPointer->m_windowed = !m_selfPointer->m_windowed;
	m_selfPointer->m_graphicsWrapper->changeToWindowed(m_selfPointer->m_windowed);
}

void TW_CALL GraphicsBackendSystem::applyNewResolution( void* p_clientData )
{
	m_selfPointer->m_scrWidth = m_selfPointer->m_newWidth;
	m_selfPointer->m_scrHeight = m_selfPointer->m_newHeight;

	m_selfPointer->m_window->changeWindowRes(  m_selfPointer->m_scrWidth,
		m_selfPointer->m_scrHeight );
	m_selfPointer->m_graphicsWrapper->changeBackbufferRes( m_selfPointer->m_scrWidth,
		m_selfPointer->m_scrHeight );

	Entity* mainCamera =
		m_selfPointer->m_world->getEntityManager()->getFirstEntityByComponentType(
		ComponentType::MainCamera );
	if( mainCamera != NULL )
	{
		
		CameraInfo* cameraInfo = static_cast<CameraInfo*>( mainCamera->getComponent(
			ComponentType::CameraInfo ) );
		cameraInfo->createPerspectiveMatrix(m_selfPointer->getAspectRatio());

	}

}

float GraphicsBackendSystem::getAspectRatio()
{
	return (float)m_scrWidth / m_scrHeight;
}

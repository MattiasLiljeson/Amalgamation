#include "LibRocketRenderSystem.h"
#include "GraphicsBackendSystem.h"
#include "libRocketBackendSystem.h"


LibRocketRenderSystem::LibRocketRenderSystem(  LibRocketBackendSystem* p_libRocketBackend  )
											   : EntitySystem( SystemType::LibRocketRenderSystem )
{
	m_libRocketBackend = p_libRocketBackend;
}


LibRocketRenderSystem::~LibRocketRenderSystem()
{
}

void LibRocketRenderSystem::process()
{
}

void LibRocketRenderSystem::render()
{
	m_libRocketBackend->render();
}

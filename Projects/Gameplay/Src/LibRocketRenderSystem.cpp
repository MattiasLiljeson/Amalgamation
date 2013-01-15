#include "LibRocketRenderSystem.h"
#include "GraphicsBackendSystem.h"
#include "libRocketBackendSystem.h"
#include <GraphicsWrapper.h>


LibRocketRenderSystem::LibRocketRenderSystem( GraphicsBackendSystem* p_gfxBackend,
											   LibRocketBackendSystem* p_libRocketBackend  )
											   : EntitySystem( SystemType::LibRocketRenderSystem )
{
	m_gfxBackend = p_gfxBackend;
	m_libRocketBackend = p_libRocketBackend;
}


LibRocketRenderSystem::~LibRocketRenderSystem()
{
}

void LibRocketRenderSystem::processEntities()
{
	GraphicsWrapper* gfxWrapper = m_gfxBackend->getGfxWrapper();

	gfxWrapper->beginGUIPass();
	m_libRocketBackend->render();
	gfxWrapper->finalizeGUIPass();
}

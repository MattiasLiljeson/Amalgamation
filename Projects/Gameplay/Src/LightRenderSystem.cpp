#include "LightRenderSystem.h"
#include "GraphicsBackendSystem.h"
#include <GraphicsWrapper.h>


LightRenderSystem::LightRenderSystem( GraphicsBackendSystem* p_gfxBackend )
	: EntitySystem( SystemType::LightRenderSystem )
{
	m_gfxBackend = p_gfxBackend;
}


LightRenderSystem::~LightRenderSystem()
{
}

void LightRenderSystem::processEntities( const vector<Entity*>& p_entities )
{
	GraphicsWrapper* gfxWrapper = m_gfxBackend->getGfxWrapper();

	gfxWrapper->flipBackBuffer();           // flip buffers
}
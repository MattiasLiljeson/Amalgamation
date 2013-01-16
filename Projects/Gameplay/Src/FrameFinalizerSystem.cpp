#include "FrameFinalizerSystem.h"
#include "GraphicsBackendSystem.h"
#include <GraphicsWrapper.h>


FrameFinalizerSystem::FrameFinalizerSystem( GraphicsBackendSystem* p_gfxBackend )
										   : EntitySystem( SystemType::FrameFinalizerSystem )
{
	m_gfxBackend = p_gfxBackend;
}


FrameFinalizerSystem::~FrameFinalizerSystem()
{
}

void FrameFinalizerSystem::process()
{
	GraphicsWrapper* gfxWrapper = m_gfxBackend->getGfxWrapper();
	gfxWrapper->flipBackBuffer();           // flip buffers
}

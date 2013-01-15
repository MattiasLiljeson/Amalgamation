#include "ParticleRenderSystem.h"
#include "GraphicsBackendSystem.h"


ParticleRenderSystem::ParticleRenderSystem( GraphicsBackendSystem* p_gfxBackend )
										   : EntitySystem( SystemType::ParticleRenderSystem )
{
}


ParticleRenderSystem::~ParticleRenderSystem()
{
}

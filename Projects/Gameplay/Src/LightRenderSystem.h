#pragma once
#include <EntitySystem.h>

class GraphicsBackendSystem;

class LightRenderSystem : public EntitySystem
{
public:
	LightRenderSystem( GraphicsBackendSystem* p_gfxBackend );
	~LightRenderSystem();
};


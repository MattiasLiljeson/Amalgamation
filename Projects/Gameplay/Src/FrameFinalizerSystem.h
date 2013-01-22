#pragma once
#include <EntitySystem.h>

class GraphicsBackendSystem;

class FrameFinalizerSystem : public EntitySystem
{
public:
	FrameFinalizerSystem( GraphicsBackendSystem* p_gfxBackend );
	~FrameFinalizerSystem();

	virtual void process();

private:
	GraphicsBackendSystem* m_gfxBackend;
};


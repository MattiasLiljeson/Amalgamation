#pragma once
#include <EntitySystem.h>

class GraphicsBackendSystem;

class LightRenderSystem : public EntitySystem
{
public:
	LightRenderSystem( GraphicsBackendSystem* p_gfxBackend );
	~LightRenderSystem();

	virtual void processEntities( const vector<Entity*>& p_entities );


private:
	GraphicsBackendSystem* m_gfxBackend;
};


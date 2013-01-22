#pragma once
#include <EntitySystem.h>

class GraphicsBackendSystem;
class LightMesh;

class LightRenderSystem : public EntitySystem
{
public:
	LightRenderSystem( GraphicsBackendSystem* p_gfxBackend );
	~LightRenderSystem();

	virtual void processEntities( const vector<Entity*>& p_entities );

	virtual void initialize();



private:
	GraphicsBackendSystem* m_gfxBackend;
	LightMesh* m_box;
};


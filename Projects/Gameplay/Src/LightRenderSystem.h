#pragma once
#include <EntitySystem.h>

//tmp
#include "..\..\Graphics\Src\PTVertex.h"
#include "..\..\Graphics\Src\Buffer.h"

class GraphicsBackendSystem;

class LightRenderSystem : public EntitySystem
{
public:
	LightRenderSystem( GraphicsBackendSystem* p_gfxBackend );
	~LightRenderSystem();

	virtual void processEntities( const vector<Entity*>& p_entities );

	virtual void initialize();



private:
	GraphicsBackendSystem* m_gfxBackend;
	int m_boxId;
};


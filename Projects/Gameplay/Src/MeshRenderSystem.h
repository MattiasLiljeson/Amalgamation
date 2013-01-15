#pragma once

#include <EntitySystem.h>

class GraphicsBackendSystem;
struct InstanceData;

class MeshRenderSystem : public EntitySystem
{
public:
	MeshRenderSystem(  GraphicsBackendSystem* p_gfxBackend );
	virtual ~MeshRenderSystem();
	virtual void initialize();

	virtual void processEntities( const vector<Entity*>& p_entities );

private:
	vector< vector<InstanceData> > m_instanceLists;
	GraphicsBackendSystem* m_gfxBackend;
};


#pragma once

#include <EntitySystem.h>
#include <InstanceData.h>

class GraphicsBackendSystem;
class LibRocketBackendSystem;

class MeshRenderSystem : public EntitySystem
{
public:
	MeshRenderSystem(  GraphicsBackendSystem* p_gfxBackend,
		LibRocketBackendSystem* p_rocketBackend );
	virtual ~MeshRenderSystem();
	virtual void initialize();

	virtual void processEntities( const vector<Entity*>& p_entities );

private:
	vector< vector<InstanceData> > m_instanceLists;
	GraphicsBackendSystem* m_gfxBackend;
	LibRocketBackendSystem* m_libRocketBackend;

};


#pragma once

#include <EntitySystem.h>
#include <RenderInterface.h>
#include <AglVector4.h>
#include <AglOBB.h>

class GraphicsBackendSystem;
class RenderInfo;
struct AglMatrix;
struct InstanceData;
// =======================================================================================
//                                      MeshRenderSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # MeshRenderSystem
/// Detailed description.....
/// Created on: 22-1-2013 
///---------------------------------------------------------------------------------------

class MeshRenderSystem : public EntitySystem, public RenderInterface
{
public:
	MeshRenderSystem(  GraphicsBackendSystem* p_gfxBackend );
	virtual ~MeshRenderSystem();
	virtual void initialize();

	virtual void processEntities( const vector<Entity*>& p_entities );

	virtual void render();

private:
	void fillInstanceData(InstanceData* p_data, Entity* p_entity, RenderInfo* p_renderInfo);

	RenderInfo* getRenderInfo(Entity* p_entity);
private:
	vector< vector<InstanceData> > m_instanceLists;
	vector< vector<InstanceData> > m_instanceListsTess;

	vector< vector<AglMatrix> > m_boneMatrices;
	GraphicsBackendSystem* m_gfxBackend;
};


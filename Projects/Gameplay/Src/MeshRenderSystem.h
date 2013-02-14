#pragma once

#include <EntitySystem.h>
#include <RenderInterface.h>

class GraphicsBackendSystem;
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
	vector< vector<InstanceData> > m_instanceLists;
	vector< vector<AglMatrix> > m_boneMatrices;
	GraphicsBackendSystem* m_gfxBackend;
};


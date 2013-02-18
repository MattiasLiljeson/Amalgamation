#pragma once

#include <EntitySystem.h>
#include <RenderInterface.h>
#include "Transform.h"
#include <AglVector4.h>
#include <AglOBB.h>

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

	unsigned int* getCulledCountPtr()
	{
		return &m_culled;
	}
	unsigned int* getRenderedCountPtr()
	{
		return &m_rendered;
	}
	float* getCulledFractionPtr()
	{
		return &m_culledFraction;
	}

private:
	void fillInstanceData(InstanceData* p_data, Entity* p_entity);

	bool shouldCull(Entity* p_entity);
	void calcCameraPlanes();

	bool BoxPlane(const AglOBB& p_box, const AglVector4& p_plane);
private:
	vector< vector<InstanceData> > m_instanceLists;
	vector< vector<AglMatrix> > m_boneMatrices;
	GraphicsBackendSystem* m_gfxBackend;

	//For culling
	AglVector4 m_cameraPlanes[6];

	unsigned int m_culled;
	unsigned int m_rendered;
	float m_culledFraction;
};


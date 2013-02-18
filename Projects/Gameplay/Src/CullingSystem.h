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

class CullingSystem : public EntitySystem
{
public:
	CullingSystem();
	virtual ~CullingSystem();
	virtual void initialize();

	virtual void processEntities( const vector<Entity*>& p_entities );

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
	bool shouldCull(Entity* p_entity);
	void calcCameraPlanes();
	bool BoxPlane(const AglOBB& p_box, const AglVector4& p_plane);
private:
	AglVector4 m_cameraPlanes[6];

	unsigned int m_culled;
	unsigned int m_rendered;
	float m_culledFraction;
};


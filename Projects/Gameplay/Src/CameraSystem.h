#pragma once

#include <EntitySystem.h>

class GraphicsBackendSystem;
class InputBackendSystem;

// =======================================================================================
//                                      CameraSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # CameraSystem
/// Detailed description.....
/// Created on: 20-12-2012 
///---------------------------------------------------------------------------------------

class CameraSystem : public EntitySystem
{
public:
	CameraSystem( GraphicsBackendSystem* p_gfxBackend );
	~CameraSystem();

	virtual void initialize();
	virtual void processEntities( const vector<Entity*>& p_entities );
	void updateRenderSceneInfo(Entity* p_entity);
	void setRenderSceneInfoUsingPlayerCamera();
	bool selectPlayerCamera( Entity* p_camera );
private:
	GraphicsBackendSystem* m_gfxBackend;
	bool	m_renderFromShadowCamera;
	Entity* m_playerCamera;
	int m_lowResDivider;
};


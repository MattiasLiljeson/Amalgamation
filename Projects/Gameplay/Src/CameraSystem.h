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
private:
	void updateRenderSceneInfo(Entity* p_entity);
private:
	GraphicsBackendSystem* m_gfxBackend;
};


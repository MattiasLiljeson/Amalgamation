#pragma once
#include <EntitySystem.h>


class GraphicsBackendSystem;
class LibRocketBackendSystem;

// =======================================================================================
//                                      LibRocketRenderSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # LibRocketRenderSystem
/// Detailed description.....
/// Created on: 15-1-2013 
///---------------------------------------------------------------------------------------
class LibRocketRenderSystem : public EntitySystem
{
public:
	LibRocketRenderSystem( GraphicsBackendSystem* p_gfxBackend,
		LibRocketBackendSystem* p_rocketBackend  );
	~LibRocketRenderSystem();

	virtual void processEntities();

private:
	GraphicsBackendSystem* m_gfxBackend;
	LibRocketBackendSystem* m_libRocketBackend 
};


#pragma once
#include <EntitySystem.h>
#include <RenderInterface.h>

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
class LibRocketRenderSystem : public EntitySystem, public RenderInterface
{
public:
	LibRocketRenderSystem( LibRocketBackendSystem* p_rocketBackend );
	~LibRocketRenderSystem();

	virtual void process();
	virtual void render();
private:
	LibRocketBackendSystem* m_libRocketBackend;
};


#pragma once
#include <EntitySystem.h>

class RenderInterface;
class GraphicsBackendSystem;

// =======================================================================================
//                                      GraphicsRendererSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Handles rendering of all the systems that implements RenderInterface in a 
/// structured way.
///        
/// # GraphicsRendererSystem
/// Detailed description.....
/// Created on: 22-1-2013 
///---------------------------------------------------------------------------------------

class GraphicsRendererSystem : public EntitySystem
{
public:
	GraphicsRendererSystem(GraphicsBackendSystem* p_graphicsBackend,
		RenderInterface* p_mesh, RenderInterface* p_libRocket, RenderInterface* p_particle,
		RenderInterface* p_antTweakBar, RenderInterface* p_light);
	virtual ~GraphicsRendererSystem();

	virtual void initialize();
	virtual void process();
	
	void initMeshPass();
	void endMeshPass();

	void initParticlePass();
	void endParticlePass();

	void initLightPass();
	void endLightPass();
	
	void initGUIPass();
	void endGUIPass();
	
	void flipBackbuffer();
private:
	GraphicsBackendSystem* m_backend;
	RenderInterface* m_meshRenderer;
	RenderInterface* m_libRocketRenderSystem;
	RenderInterface* m_particleRenderSystem;
	RenderInterface* m_antTweakBarSystem;
	RenderInterface* m_lightRenderSystem;
};
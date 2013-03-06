#pragma once
#include <EntitySystem.h>
#include <string>
#include <GPUTimerProfile.h>

class RenderInterface;
class GraphicsBackendSystem;
class GraphicsWrapper;
class ShadowSystem;

struct AglMatrix;

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
		ShadowSystem* p_shadowSystem, RenderInterface* p_mesh, 
		RenderInterface* p_libRocket, RenderInterface* p_particle,
		RenderInterface* p_antTweakBar, RenderInterface* p_light);
	virtual ~GraphicsRendererSystem();

	virtual void initialize();
	virtual void process();

	void initShadowPass();
	void endShadowPass();

	void initMeshPass();
	void endMeshPass();

	void initLightPass();
	void endLightPass();

	void beginSsao();
	void endSsao();

	void beginDofGenerationPass();
	void endDofGenerationPass();

	void initComposePass();
	void endComposePass();
	
	void initParticlePass();
	void renderParticles();
	void endParticlePass();

	void initGUIPass();
	void endGUIPass();
	
	void flipBackbuffer();

	void clearShadowStuf();

	void updateTimers();
private:
	enum {
		SHADOW, MESH, LIGHT, SSAO, DOF, COMPOSE, PARTICLE, GUI, NUMRENDERINGPASSES
	};
private:
	GraphicsWrapper* m_wrapper;
	GraphicsBackendSystem* m_backend;
	ShadowSystem*	 m_shadowSystem;
	RenderInterface* m_meshRenderer;
	RenderInterface* m_libRocketRenderSystem;
	RenderInterface* m_particleRenderSystem;
	RenderInterface* m_antTweakBarSystem;
	RenderInterface* m_lightRenderSystem;

	int* m_activeShadows;
	AglMatrix* m_shadowViewProjections;

	vector<GPUTimerProfile> m_profiles;
	double	m_totalTime;

	int		m_currentFrame;
};
#pragma once
#include <EntitySystem.h>
#include <string>

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

	void initParticlePass();
	void endParticlePass();

	void initLightPass();
	void endLightPass();

	void beginSsao();
	void endSsao();

	void initComposePass();
	void endComposePass();
	
	void initGUIPass();
	void endGUIPass();
	
	void flipBackbuffer();

	void clearShadowStuf();

	void updateTimers();
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

	std::string m_shadowProfile;
	std::string m_meshProfile;
	std::string m_lightProfile;
	std::string m_composeProfile;
	std::string m_particleProfile;
	std::string m_guiProfile;

	double	m_shadowPassTime;
	double	m_meshPassTime;
	double	m_lightPassTime;
	double	m_composePassTime;
	double	m_particlePassTime;
	double	m_guiPassTime;
	double	m_totalTime;

	int		m_currentFrame;
};
#include "GraphicsRendererSystem.h"
#include <RenderInterface.h>
#include "GraphicsBackendSystem.h"
#include <GraphicsWrapper.h>

GraphicsRendererSystem::GraphicsRendererSystem(GraphicsBackendSystem* p_graphicsBackend, 
											   RenderInterface* p_mesh, 
											   RenderInterface* p_libRocket, 
											   RenderInterface* p_particle, 
											   RenderInterface* p_antTweakBar, 
											   RenderInterface* p_light) 
											   : EntitySystem(
											   SystemType::GraphicsRendererSystem){
	m_backend				= p_graphicsBackend;
	m_meshRenderer			= p_mesh;
	m_libRocketRenderSystem = p_libRocket;
	m_particleRenderSystem	= p_particle;
	m_antTweakBarSystem		= p_antTweakBar;
	m_lightRenderSystem		= p_light;
}
GraphicsRendererSystem::~GraphicsRendererSystem(){

}
void GraphicsRendererSystem::initialize(){

}
void GraphicsRendererSystem::process(){
	initMeshPass();
	m_meshRenderer->render();
	endMeshPass();

	initParticlePass();
	m_particleRenderSystem->render();
	endParticlePass();

	initLightPass();
	m_lightRenderSystem->render();
	endLightPass();

	initGUIPass();
	m_libRocketRenderSystem->render();
	m_antTweakBarSystem->render();
	endGUIPass();

	flipBackbuffer();
}
void GraphicsRendererSystem::initMeshPass(){
	m_backend->getGfxWrapper()->clearRenderTargets();
	m_backend->getGfxWrapper()->beginFrame();
}
void GraphicsRendererSystem::endMeshPass(){

}

void GraphicsRendererSystem::initParticlePass(){

}

void GraphicsRendererSystem::endParticlePass(){

}

void GraphicsRendererSystem::initLightPass(){
	m_backend->getGfxWrapper()->beginLightPass();
}

void GraphicsRendererSystem::endLightPass(){
	m_backend->getGfxWrapper()->endLightPass();
}

void GraphicsRendererSystem::initGUIPass(){
	m_backend->getGfxWrapper()->beginGUIPass();
}

void GraphicsRendererSystem::endGUIPass(){
	m_backend->getGfxWrapper()->finalizeGUIPass();
}

void GraphicsRendererSystem::flipBackbuffer(){
	m_backend->getGfxWrapper()->flipBackBuffer();
}



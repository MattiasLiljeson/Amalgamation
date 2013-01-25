#include "GraphicsRendererSystem.h"
#include <RenderInterface.h>
#include "GraphicsBackendSystem.h"
#include <GraphicsWrapper.h>
#include <RenderStateEnums.h>

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

	initLightPass();
	m_lightRenderSystem->render();
	endLightPass();

	initParticlePass();
	m_particleRenderSystem->render();
	endParticlePass();

	initGUIPass();
	m_antTweakBarSystem->render();
	m_libRocketRenderSystem->render();
	endGUIPass();

	flipBackbuffer();
}
void GraphicsRendererSystem::initMeshPass(){
	m_backend->getGfxWrapper()->clearRenderTargets();
	m_backend->getGfxWrapper()->setBaseRenderTargets();
	m_backend->getGfxWrapper()->setRasterizerStateSettings(RasterizerState::DEFAULT);
	m_backend->getGfxWrapper()->setBlendStateSettings(BlendState::DEFAULT);
	m_backend->getGfxWrapper()->setPrimitiveTopology(PrimitiveTopology::TRIANGLELIST);
	m_backend->getGfxWrapper()->mapSceneInfo();
}

void GraphicsRendererSystem::endMeshPass(){
}

void GraphicsRendererSystem::initLightPass(){
	m_backend->getGfxWrapper()->setRasterizerStateSettings(
		RasterizerState::FILLED_CW_FRONTCULL, false);
	m_backend->getGfxWrapper()->setBlendStateSettings(BlendState::ADDITIVE);
	m_backend->getGfxWrapper()->setComposedRenderTargetWithNoDepthStencil();
	m_backend->getGfxWrapper()->mapGBuffersToShader();
}

void GraphicsRendererSystem::endLightPass(){
	m_backend->getGfxWrapper()->setRasterizerStateSettings(RasterizerState::DEFAULT);
	m_backend->getGfxWrapper()->setBlendStateSettings(BlendState::DEFAULT);
}

void GraphicsRendererSystem::initParticlePass(){
	m_backend->getGfxWrapper()->unmapDepthFromShader();
	m_backend->getGfxWrapper()->setParticleRenderState();
	m_backend->getGfxWrapper()->setBlendStateSettings(BlendState::PARTICLE);
	m_backend->getGfxWrapper()->setPrimitiveTopology(PrimitiveTopology::POINTLIST);
}

void GraphicsRendererSystem::endParticlePass(){
	m_backend->getGfxWrapper()->setPrimitiveTopology(PrimitiveTopology::TRIANGLELIST);
	m_backend->getGfxWrapper()->setBlendStateSettings(BlendState::DEFAULT);
	m_backend->getGfxWrapper()->setComposedRenderTargetWithNoDepthStencil();
}

void GraphicsRendererSystem::initGUIPass(){
	m_backend->getGfxWrapper()->setBlendStateSettings(BlendState::ALPHA);
}

void GraphicsRendererSystem::endGUIPass(){
	m_backend->getGfxWrapper()->setBlendStateSettings(BlendState::DEFAULT);
}

void GraphicsRendererSystem::flipBackbuffer(){
	m_backend->getGfxWrapper()->flipBackBuffer();
}



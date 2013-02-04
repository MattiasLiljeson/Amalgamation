#include "GraphicsRendererSystem.h"
#include <RenderInterface.h>
#include "GraphicsBackendSystem.h"
#include <GraphicsWrapper.h>
#include <RenderStateEnums.h>
#include "ShadowSystem.h"

GraphicsRendererSystem::GraphicsRendererSystem(GraphicsBackendSystem* p_graphicsBackend,
											   ShadowSystem*	p_shadowSystem,
											   RenderInterface* p_mesh, 
											   RenderInterface* p_libRocket, 
											   RenderInterface* p_particle, 
											   RenderInterface* p_antTweakBar, 
											   RenderInterface* p_light) 
											   : EntitySystem(
											   SystemType::GraphicsRendererSystem){
	m_backend				= p_graphicsBackend;
	m_shadowSystem			= p_shadowSystem;
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
	m_wrapper = m_backend->getGfxWrapper();

	
	//initShadowPass();
	//for(unsigned int i = 0; i < m_shadowSystem->getNumberOfShadowCameras(); i++){
	//	AglMatrix vp = m_shadowSystem->getViewProjection(i);
	//	m_wrapper->setShadowViewProjection(vp);
	//	m_meshRenderer->render();
	//}
	//endShadowPass();
	
	initMeshPass();
	m_wrapper->setRasterizerStateSettings(RasterizerState::FILLED_NOCULL);
	m_meshRenderer->render();
	endMeshPass();

	initLightPass();
	m_lightRenderSystem->render();
	endLightPass();

	initComposePass();
	m_wrapper->renderComposeStage();
	endComposePass();

	initParticlePass();
	m_particleRenderSystem->render();
	endParticlePass();

	initGUIPass();
	m_antTweakBarSystem->render();
	m_libRocketRenderSystem->render();
	endGUIPass();

	flipBackbuffer();
}
void GraphicsRendererSystem::initShadowPass(){

	m_wrapper->setShadowMapAsRenderTarget();
	m_wrapper->setViewportToShadowMapSize();
	m_wrapper->setRenderingShadows();
}

void GraphicsRendererSystem::endShadowPass(){
	m_wrapper->resetViewportToOriginalSize();
	m_wrapper->stopedRenderingShadows();
}
void GraphicsRendererSystem::initMeshPass(){
	m_wrapper->setRasterizerStateSettings(RasterizerState::DEFAULT);
	m_wrapper->setBlendStateSettings(BlendState::DEFAULT);
	m_wrapper->setPrimitiveTopology(PrimitiveTopology::TRIANGLELIST);
	m_wrapper->clearRenderTargets();
	m_wrapper->setBaseRenderTargets();
	m_wrapper->mapSceneInfo();
}

void GraphicsRendererSystem::endMeshPass(){
}

void GraphicsRendererSystem::initLightPass(){
	m_wrapper->setRasterizerStateSettings(
		RasterizerState::FILLED_NOCULL_NOCLIP, false);
	m_wrapper->setBlendStateSettings(BlendState::ADDITIVE);
	m_wrapper->setLightPassRenderTarget();
	m_wrapper->mapDeferredBaseToShader();
}

void GraphicsRendererSystem::endLightPass(){
	m_wrapper->setRasterizerStateSettings(RasterizerState::DEFAULT);
	m_wrapper->setBlendStateSettings(BlendState::DEFAULT);
}

void GraphicsRendererSystem::initComposePass()
{
	m_wrapper->setPrimitiveTopology(PrimitiveTopology::TRIANGLESTRIP);
	m_wrapper->setComposedRenderTargetWithNoDepthStencil();
	m_wrapper->mapVariousStagesForCompose();
}

void GraphicsRendererSystem::endComposePass()
{
	m_wrapper->setPrimitiveTopology(PrimitiveTopology::TRIANGLELIST);
	m_wrapper->unmapVariousStagesForCompose();
	m_wrapper->unmapDepthFromShader();
}

void GraphicsRendererSystem::initParticlePass(){
	m_wrapper->setParticleRenderState();
	m_wrapper->setBlendStateSettings(BlendState::PARTICLE);
	m_wrapper->setPrimitiveTopology(PrimitiveTopology::POINTLIST);
}

void GraphicsRendererSystem::endParticlePass(){
	m_wrapper->setPrimitiveTopology(PrimitiveTopology::TRIANGLELIST);
	m_wrapper->setBlendStateSettings(BlendState::DEFAULT);
	m_wrapper->setComposedRenderTargetWithNoDepthStencil();
}

void GraphicsRendererSystem::initGUIPass(){
	m_wrapper->setBlendStateSettings(BlendState::ALPHA);
}

void GraphicsRendererSystem::endGUIPass(){
	m_wrapper->setBlendStateSettings(BlendState::DEFAULT);
}

void GraphicsRendererSystem::flipBackbuffer(){
	m_wrapper->flipBackBuffer();
}
#include "GraphicsRendererSystem.h"
#include <RenderInterface.h>
#include "GraphicsBackendSystem.h"
#include <GraphicsWrapper.h>
#include <RenderStateEnums.h>
#include "ShadowSystem.h"
#include <GPUTimer.h>
#include <AntTweakBarWrapper.h>

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
	m_shadowPassTime		= 0.0f;

	m_activeShadows			= new int[MAXSHADOWS];
	m_shadowViewProjections = new AglMatrix[MAXSHADOWS];

	clearShadowStuf();
}
GraphicsRendererSystem::~GraphicsRendererSystem(){
	delete[] m_shadowViewProjections;
	delete[] m_activeShadows;
}
void GraphicsRendererSystem::initialize(){
	AntTweakBarWrapper::getInstance()->addReadOnlyVariable(
		AntTweakBarWrapper::MEASUREMENT,"ShadowPass",TwType::TW_TYPE_DOUBLE,
		&m_shadowPassTime,"group=GPU");
}
void GraphicsRendererSystem::process(){

	m_wrapper = m_backend->getGfxWrapper();
	m_wrapper->getGPUTimer()->Start();

	clearShadowStuf();
	//Fill the shadow view projections
	for (unsigned int i = 0; i < m_shadowSystem->getNumberOfShadowCameras(); i++){
		m_activeShadows[m_shadowSystem->getShadowIdx(i)] = 1;
		m_shadowViewProjections[m_shadowSystem->getShadowIdx(i)] = 
			m_shadowSystem->getViewProjection(i);
	}

	initShadowPass();
	for(unsigned int i = 0; i < MAXSHADOWS; i++){
		if(m_activeShadows[i] != -1){
			m_wrapper->setShadowMapAsRenderTarget(i);
			//m_wrapper->setActiveShadow(i);
			m_meshRenderer->render();
		}
	}
	endShadowPass();
	m_wrapper->getGPUTimer()->Stop();
	
	initMeshPass();
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
	
	m_shadowPassTime = m_wrapper->getGPUTimer()->GetTime();

	flipBackbuffer();
}
void GraphicsRendererSystem::initShadowPass(){
	m_wrapper->setRasterizerStateSettings(RasterizerState::FILLED_CW_FRONTCULL);
	m_wrapper->setBlendStateSettings(BlendState::DEFAULT);
	m_wrapper->setPrimitiveTopology(PrimitiveTopology::TRIANGLELIST);
	m_wrapper->setViewportToShadowMapSize();
	m_wrapper->setRenderingShadows();
	m_wrapper->setShadowViewProjections(m_shadowViewProjections);
}

void GraphicsRendererSystem::endShadowPass(){
	m_wrapper->resetViewportToOriginalSize();
	m_wrapper->stopedRenderingShadows();
	//m_wrapper->unmapPerShadowBuffer();
}
void GraphicsRendererSystem::initMeshPass(){
	m_wrapper->mapSceneInfo();
	m_wrapper->setRasterizerStateSettings(RasterizerState::DEFAULT);
	m_wrapper->setBlendStateSettings(BlendState::DEFAULT);
	//m_wrapper->setPrimitiveTopology(PrimitiveTopology::TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
	m_wrapper->clearRenderTargets();
	m_wrapper->setBaseRenderTargets();
}

void GraphicsRendererSystem::endMeshPass(){
		m_wrapper->setPrimitiveTopology(PrimitiveTopology::TRIANGLELIST);
}

void GraphicsRendererSystem::initLightPass(){
	m_wrapper->setRasterizerStateSettings(
		RasterizerState::FILLED_NOCULL_NOCLIP, false);
	m_wrapper->setBlendStateSettings(BlendState::ADDITIVE);
	m_wrapper->setLightPassRenderTarget();
	//m_wrapper->mapDeferredBaseToShader();
	m_wrapper->mapNeededShaderResourceToLightPass(m_activeShadows);
}

void GraphicsRendererSystem::endLightPass(){
	m_wrapper->setRasterizerStateSettings(RasterizerState::DEFAULT);
	m_wrapper->setBlendStateSettings(BlendState::DEFAULT);
	//m_wrapper->unmapDeferredBaseFromShader();
	m_wrapper->unmapUsedShaderResourceFromLightPass(m_activeShadows);
}

void GraphicsRendererSystem::initComposePass()
{
	m_wrapper->setRasterizerStateSettings(
		RasterizerState::DEFAULT, false);
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

void GraphicsRendererSystem::clearShadowStuf()
{
	for(int i = 0; i < MAXSHADOWS; i++){
		m_activeShadows[i] = -1;
		m_shadowViewProjections[i] = AglMatrix::identityMatrix();
	}
}

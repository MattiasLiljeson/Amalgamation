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

	m_shadowPassTime = m_meshPassTime = m_lightPassTime = m_composePassTime 
		= m_particlePassTime = m_guiPassTime = m_totalTime =0.0f;	

	m_currentFrame			= 0;

	m_activeShadows			= new int[MAXSHADOWS];
	m_shadowViewProjections = new AglMatrix[MAXSHADOWS];

	m_shadowProfile = "Shadow";
	m_meshProfile	= "Mesh";
	m_lightProfile  = "Light";
	m_composeProfile= "Compose";
	m_particleProfile="Particle";
	m_guiProfile	= "GUI";

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
	AntTweakBarWrapper::getInstance()->addReadOnlyVariable(
		AntTweakBarWrapper::MEASUREMENT,"MeshPass",TwType::TW_TYPE_DOUBLE,
		&m_meshPassTime,"group=GPU");
	AntTweakBarWrapper::getInstance()->addReadOnlyVariable(
		AntTweakBarWrapper::MEASUREMENT,"LightPass",TwType::TW_TYPE_DOUBLE,
		&m_lightPassTime,"group=GPU");
	AntTweakBarWrapper::getInstance()->addReadOnlyVariable(
		AntTweakBarWrapper::MEASUREMENT,"ComposePass",TwType::TW_TYPE_DOUBLE,
		&m_composePassTime,"group=GPU");
	AntTweakBarWrapper::getInstance()->addReadOnlyVariable(
		AntTweakBarWrapper::MEASUREMENT,"ParticlePass",TwType::TW_TYPE_DOUBLE,
		&m_particlePassTime,"group=GPU");
	AntTweakBarWrapper::getInstance()->addReadOnlyVariable(
		AntTweakBarWrapper::MEASUREMENT,"GUIPass",TwType::TW_TYPE_DOUBLE,
		&m_guiPassTime,"group=GPU");
	AntTweakBarWrapper::getInstance()->addReadOnlyVariable(
		AntTweakBarWrapper::MEASUREMENT,"Total",TwType::TW_TYPE_DOUBLE,
		&m_totalTime,"group=GPU");


	m_backend->getGfxWrapper()->getGPUTimer()->addProfile(m_shadowProfile);
	m_backend->getGfxWrapper()->getGPUTimer()->addProfile(m_meshProfile);
	m_backend->getGfxWrapper()->getGPUTimer()->addProfile(m_lightProfile);
	m_backend->getGfxWrapper()->getGPUTimer()->addProfile(m_composeProfile);
	m_backend->getGfxWrapper()->getGPUTimer()->addProfile(m_particleProfile);
	m_backend->getGfxWrapper()->getGPUTimer()->addProfile(m_guiProfile);
}
void GraphicsRendererSystem::process(){

	m_wrapper = m_backend->getGfxWrapper();

	//Shadows
	m_wrapper->getGPUTimer()->Start(m_shadowProfile, m_currentFrame);
	clearShadowStuf();
	//Fill the shadow view projections
	for (unsigned int i = 0; i < m_shadowSystem->getNumberOfShadowCameras(); i++){
		m_activeShadows[m_shadowSystem->getShadowIdx(i)] = 1;
		m_shadowViewProjections[m_shadowSystem->getShadowIdx(i)] = 
			m_shadowSystem->getViewProjection(i);
	}

	/*
	initShadowPass();
	for(unsigned int i = 0; i < MAXSHADOWS; i++){
		if(m_activeShadows[i] != -1){
			m_wrapper->setShadowMapAsRenderTarget(i);
			//m_wrapper->setActiveShadow(i);
			m_meshRenderer->render();
		}
	}
	endShadowPass();
	*/
	m_wrapper->getGPUTimer()->Stop(m_shadowProfile, m_currentFrame);

	// Meshes
	m_wrapper->getGPUTimer()->Start(m_meshProfile, m_currentFrame);
	initMeshPass();
	m_wrapper->setRasterizerStateSettings(RasterizerState::FILLED_CW);
	m_meshRenderer->render();
	endMeshPass();
	m_wrapper->getGPUTimer()->Stop(m_meshProfile, m_currentFrame);

	// Lights
	m_wrapper->getGPUTimer()->Start(m_lightProfile, m_currentFrame);
	initLightPass();
	m_lightRenderSystem->render();
	endLightPass();
	m_wrapper->getGPUTimer()->Stop(m_lightProfile, m_currentFrame);

	//SSAO
	beginSsao();
	m_wrapper->renderSsao();
	endSsao();

	//Compose
	m_wrapper->getGPUTimer()->Start(m_composeProfile, m_currentFrame);
	initComposePass();
	m_wrapper->renderComposeStage();
	endComposePass();
	m_wrapper->getGPUTimer()->Stop(m_composeProfile, m_currentFrame);

	//Particles
	m_wrapper->getGPUTimer()->Start(m_particleProfile, m_currentFrame);
	initParticlePass();
	m_particleRenderSystem->render();
	endParticlePass();
	m_wrapper->getGPUTimer()->Stop(m_particleProfile, m_currentFrame);
	
	//GUI
	m_wrapper->getGPUTimer()->Start(m_guiProfile, m_currentFrame);
	initGUIPass();
	m_antTweakBarSystem->render();
	m_libRocketRenderSystem->render();
	endGUIPass();
	m_wrapper->getGPUTimer()->Stop(m_guiProfile, m_currentFrame);

	flipBackbuffer();

	updateTimers();
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
	m_wrapper->setBlendStateSettings(BlendState::LIGHT);
	m_wrapper->setLightPassRenderTarget();
	//m_wrapper->mapDeferredBaseToShader();
	m_wrapper->mapNeededShaderResourceToLightPass(m_activeShadows);
}

void GraphicsRendererSystem::endLightPass(){
	//m_wrapper->setRasterizerStateSettings(RasterizerState::DEFAULT);
	//m_wrapper->setBlendStateSettings(BlendState::DEFAULT);
	//m_wrapper->unmapDeferredBaseFromShader();
	//m_wrapper->unmapUsedShaderResourceFromLightPass(m_activeShadows);
}

void GraphicsRendererSystem::beginSsao()
{
	// not used anymore
	//m_wrapper->mapRandomVecTexture();
	m_wrapper->setPrimitiveTopology(PrimitiveTopology::TRIANGLESTRIP);
	m_wrapper->setBlendStateSettings(BlendState::SSAO);
	//m_wrapper->setRasterizerStateSettings(
	//	RasterizerState::FILLED_NOCULL_NOCLIP, false);
}

void GraphicsRendererSystem::endSsao()
{
	m_wrapper->setRasterizerStateSettings(RasterizerState::DEFAULT);
	m_wrapper->setBlendStateSettings(BlendState::DEFAULT);
	m_wrapper->unmapDeferredBaseFromShader();
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

void GraphicsRendererSystem::updateTimers()
{
	if(m_currentFrame == 0){
		m_currentFrame = 1;
	}
	else if(m_currentFrame==1){
		m_currentFrame = 0;
	}
	m_totalTime = 0;
	GPUTimer* timer = m_wrapper->getGPUTimer();
	m_meshPassTime = timer->getTheTimeAndReset(m_meshProfile,m_currentFrame);
	m_shadowPassTime = timer->getTheTimeAndReset(m_shadowProfile,m_currentFrame);
	m_lightPassTime = timer->getTheTimeAndReset(m_lightProfile,m_currentFrame);
	m_composePassTime = timer->getTheTimeAndReset(m_composeProfile, m_currentFrame);
	m_particlePassTime = timer->getTheTimeAndReset(m_particleProfile, m_currentFrame);
	m_guiPassTime = timer->getTheTimeAndReset(m_guiProfile, m_currentFrame);
	m_totalTime += m_meshPassTime+m_shadowPassTime+m_lightPassTime+m_composePassTime+
		m_particlePassTime+m_guiPassTime;
}

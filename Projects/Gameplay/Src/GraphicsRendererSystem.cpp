#include "GraphicsRendererSystem.h"
#include <RenderInterface.h>
#include "GraphicsBackendSystem.h"
#include <GraphicsWrapper.h>
#include <RenderStateEnums.h>
#include "ShadowSystem.h"
#include <GPUTimer.h>
#include <AntTweakBarWrapper.h>
#include "ParticleRenderSystem.h"

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

	m_totalTime =	0.0f;	

	m_activeShadows			= new int[MAXSHADOWS];
	m_shadowViewProjections = new AglMatrix[MAXSHADOWS];

	m_profiles.push_back(GPUTimerProfile("SHADOW"));
	m_profiles.push_back(GPUTimerProfile("MESH"));
	m_profiles.push_back(GPUTimerProfile("LIGHT"));
	m_profiles.push_back(GPUTimerProfile("SSAO"));
	m_profiles.push_back(GPUTimerProfile("COMPOSE"));
	m_profiles.push_back(GPUTimerProfile("PARTICLE"));
	m_profiles.push_back(GPUTimerProfile("GUI"));

	clearShadowStuf();
}
GraphicsRendererSystem::~GraphicsRendererSystem(){
	delete[] m_shadowViewProjections;
	delete[] m_activeShadows;
}
void GraphicsRendererSystem::initialize(){
	for (unsigned int i=0;i < NUMRENDERINGPASSES; i++)
	{
		AntTweakBarWrapper::getInstance()->addReadOnlyVariable(
			AntTweakBarWrapper::MEASUREMENT,
			m_profiles[i].profile.c_str(),TwType::TW_TYPE_DOUBLE,
			&m_profiles[i].renderingTime,"group=GPU");

		m_backend->getGfxWrapper()->getGPUTimer()->addProfile(m_profiles[i].profile);
	}

	AntTweakBarWrapper::getInstance()->addReadOnlyVariable(
		AntTweakBarWrapper::MEASUREMENT,"Total",TwType::TW_TYPE_DOUBLE, &m_totalTime,"group=GPU");
	
}
void GraphicsRendererSystem::process(){

	m_wrapper = m_backend->getGfxWrapper();

	//Shadows
	//m_wrapper->getGPUTimer()->Start(m_profiles[SHADOW].profile);
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
	//m_wrapper->getGPUTimer()->Stop(m_profiles[SHADOW].profile);

	// Meshes
	//m_wrapper->getGPUTimer()->Start(m_profiles[MESH].profile);
	initMeshPass();
	m_meshRenderer->render();
	endMeshPass();
	//m_wrapper->getGPUTimer()->Stop(m_profiles[MESH].profile);

	// Lights
	//m_wrapper->getGPUTimer()->Start(m_profiles[LIGHT].profile);
	initLightPass();
	m_lightRenderSystem->render();
	endLightPass();
	//m_wrapper->getGPUTimer()->Stop(m_profiles[LIGHT].profile);

	//SSAO
	//m_wrapper->getGPUTimer()->Start(m_profiles[SSAO].profile);
	beginSsao();
	m_wrapper->renderSsao();
	endSsao();
	//m_wrapper->getGPUTimer()->Stop(m_profiles[SSAO].profile);

	//Compose
	//m_wrapper->getGPUTimer()->Start(m_profiles[COMPOSE].profile);
	initComposePass();
	m_wrapper->renderComposeStage();
	endComposePass();
	//m_wrapper->getGPUTimer()->Stop(m_profiles[COMPOSE].profile);

	//Particles
	//m_wrapper->getGPUTimer()->Start(m_profiles[PARTICLE].profile);
	initParticlePass();
	renderParticles();
	endParticlePass();
	//m_wrapper->getGPUTimer()->Stop(m_profiles[PARTICLE].profile);
	
	//GUI
	//m_wrapper->getGPUTimer()->Start(m_profiles[GUI].profile);
	initGUIPass();
	m_libRocketRenderSystem->render();
	m_antTweakBarSystem->render();
	endGUIPass();
	//m_wrapper->getGPUTimer()->Stop(m_profiles[GUI].profile);

	flipBackbuffer();

	//updateTimers();
}
void GraphicsRendererSystem::initShadowPass(){
	m_wrapper->setRasterizerStateSettings(RasterizerState::FILLED_CW_FRONTCULL);
	m_wrapper->setBlendStateSettings(BlendState::DEFAULT);
	//m_wrapper->setPrimitiveTopology(PrimitiveTopology::TRIANGLELIST);
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
	//m_wrapper->setPrimitiveTopology(PrimitiveTopology::TRIANGLELIST);
	m_wrapper->clearRenderTargets();
	m_wrapper->setBaseRenderTargets();
}

void GraphicsRendererSystem::endMeshPass(){
		m_wrapper->setPrimitiveTopology(PrimitiveTopology::TRIANGLELIST);
}

void GraphicsRendererSystem::initLightPass(){
	m_wrapper->setRasterizerStateSettings(
	//RasterizerState::WIREFRAME_NOCULL, false); // For debug /ML
	RasterizerState::FILLED_CW_FRONTCULL, false);
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
	m_wrapper->setRasterizerStateSettings(
		RasterizerState::FILLED_NOCULL_NOCLIP, false);
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
	//m_wrapper->setBlendStateSettings(BlendState::PARTICLE);
	m_wrapper->setBlendStateSettings(BlendState::ADDITIVE);
	m_wrapper->setPrimitiveTopology(PrimitiveTopology::POINTLIST);
}

void GraphicsRendererSystem::renderParticles()
{
	// A ugly way to be able to call the different rendering functions. Needs refactoring /ML.
	ParticleRenderSystem* psRender = static_cast<ParticleRenderSystem*>( m_particleRenderSystem );

	for( int i=0; i<AglParticleSystemHeader::AglBlendMode_CNT; i++ ) {
		for( int j=0; j<AglParticleSystemHeader::AglRasterizerMode_CNT; j++ ) 
		{
			AglParticleSystemHeader::AglBlendMode blend = (AglParticleSystemHeader::AglBlendMode)i;
			AglParticleSystemHeader::AglRasterizerMode rast = (AglParticleSystemHeader::AglRasterizerMode)j;

			m_wrapper->setRasterizerStateSettings( psRender->rasterizerStateFromAglRasterizerMode(rast) );
			m_wrapper->setBlendStateSettings( psRender->blendStateFromAglBlendMode(blend) );
			psRender->render( blend, rast );
		}
	}
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
	m_totalTime = 0;
	GPUTimer* timer = m_wrapper->getGPUTimer();

	for(unsigned int i = 0; i < NUMRENDERINGPASSES; i++){

		m_profiles[i].renderingTime = timer->getTheTimeAndReset(m_profiles[i].profile);
		m_totalTime += m_profiles[i].renderingTime;
	}
	m_wrapper->getGPUTimer()->tick();
}
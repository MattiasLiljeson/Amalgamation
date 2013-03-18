#include "GraphicsRendererSystem.h"
#include <RenderInterface.h>
#include "GraphicsBackendSystem.h"
#include <GraphicsWrapper.h>
#include <RenderStateEnums.h>
#include "ShadowSystem.h"
#include <GPUTimer.h>
#include <AntTweakBarWrapper.h>
#include "ParticleRenderSystem.h"
#include "ClientStateSystem.h"
#include "MenuSystem.h"
#include "TimerSystem.h"
#include "SettingsSystem.h"

GraphicsRendererSystem::GraphicsRendererSystem(GraphicsBackendSystem* p_graphicsBackend,
											   RenderInterface* p_mesh, 
											   RenderInterface* p_libRocket,
											   RenderInterface* p_particle, 
											   RenderInterface* p_antTweakBar, 
											   RenderInterface* p_light,
											   GameSettingsInfo& p_settings ) 
	: EntitySystem( SystemType::GraphicsRendererSystem )
{
	m_backend						= p_graphicsBackend;
	m_meshRenderer					= p_mesh;
	m_libRocketRenderSystem			= p_libRocket;
	m_particleRenderSystem			= p_particle;
	m_antTweakBarSystem				= p_antTweakBar;
	m_lightRenderSystem				= p_light;
	m_shouldRender					= true;
	m_enteredIngamePreviousFrame	= false;
	m_enableEffects					= p_settings.enableEffects;

	m_activeShadows			= new int[MAXSHADOWS];
	m_shadowViewProjections = new AglMatrix[MAXSHADOWS];

	m_profiles.push_back(GPUTimerProfile("MESH"));
	m_profiles.push_back(GPUTimerProfile("LIGH"));
	m_profiles.push_back(GPUTimerProfile("EFFECTS"));
	m_profiles.push_back(GPUTimerProfile("LO-RES"));
	m_profiles.push_back(GPUTimerProfile("COMP"));
	m_profiles.push_back(GPUTimerProfile("PART"));
	m_profiles.push_back(GPUTimerProfile("GUI"));
	m_profiles.push_back(GPUTimerProfile("TOTAL"));

	m_measureGPU=false;

	clearShadowStuf();
}
GraphicsRendererSystem::~GraphicsRendererSystem(){
	delete[] m_shadowViewProjections;
	delete[] m_activeShadows;
}
void GraphicsRendererSystem::initialize(){
	auto settings = static_cast<SettingsSystem*>(
		m_world->getSystem(SystemType::ClientStateSystem));

	// m_measureGPU = settings->getSettings().enabledGPUMeasure;

	AntTweakBarWrapper::getInstance()->addWriteVariable(
		AntTweakBarWrapper::MEASUREMENT,
		"Enable GPU timers",TwType::TW_TYPE_BOOLCPP,
		(void*)&m_measureGPU,"group=GPU");
	for (unsigned int i=0;i < NUMRENDERINGPASSES; i++)
	{

		string variableName = m_profiles[i].profile;
		AntTweakBarWrapper::getInstance()->addReadOnlyVariable(
			AntTweakBarWrapper::MEASUREMENT,
			variableName.c_str(),TwType::TW_TYPE_DOUBLE,
			&m_profiles[i].renderingTime,"group=GPU");
 
		variableName +=" Spike";
		AntTweakBarWrapper::getInstance()->addReadOnlyVariable(
			AntTweakBarWrapper::MEASUREMENT,
			variableName.c_str(),TwType::TW_TYPE_DOUBLE,
			&m_profiles[i].renderingSpike,"group='GPU Extra'");
		
		variableName = m_profiles[i].profile;
		variableName += " Avg";
		AntTweakBarWrapper::getInstance()->addReadOnlyVariable(
			AntTweakBarWrapper::MEASUREMENT,
			variableName.c_str(),TwType::TW_TYPE_DOUBLE,
			&m_profiles[i].renderingAverage,"group='GPU Extra'");

		m_backend->getGfxWrapper()->getGPUTimer()->addProfile(m_profiles[i].profile);
	}	
}
void GraphicsRendererSystem::process(){

	m_wrapper = m_backend->getGfxWrapper();

	if (isMeasuring())
	{
		auto timerSystem = static_cast<TimerSystem*>(m_world->getSystem(SystemType::TimerSystem));
		if(timerSystem->checkTimeInterval(TimerIntervals::EverySecond)){
			for (unsigned int i= 0; i < NUMRENDERINGPASSES; i++)
			{
				m_profiles[i].calculateAvarage();
			}

		}
	}


	auto gameState = static_cast<ClientStateSystem*>(
		m_world->getSystem(SystemType::ClientStateSystem));

	if( m_shouldRender){
		renderTheScene();
	}
	if(m_enteredIngamePreviousFrame){
		auto menuSystem = static_cast<MenuSystem*>(m_world->getSystem(SystemType::MenuSystem));
		menuSystem->endLoadingState();
		m_shouldRender = true;
		m_enteredIngamePreviousFrame = false;
	}
	if(!m_shouldRender && gameState->getStateDelta(GameStates::INGAME) == EnumGameDelta::NOTCHANGED){
		m_enteredIngamePreviousFrame = true;
	}
	else if(gameState->getStateDelta(GameStates::LOADING) == EnumGameDelta::EXITTHISFRAME){
		m_shouldRender = false;
	}
}


void GraphicsRendererSystem::renderTheScene()
{
	/*
	//Shadows
	// if (isMeasuring()) m_wrapper->getGPUTimer()->Start(m_profiles[SHADOW].profile);
	clearShadowStuf();
	//Fill the shadow view projections
	for (unsigned int i = 0; i < m_shadowSystem->getNumberOfShadowCameras(); i++){
		m_activeShadows[m_shadowSystem->getShadowIdx(i)] = 1;
		m_shadowViewProjections[m_shadowSystem->getShadowIdx(i)] = 
			m_shadowSystem->getViewProjection(i);
	}
	*/
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
	// if (isMeasuring()) m_wrapper->getGPUTimer()->Stop(m_profiles[SHADOW].profile);
	m_wrapper->clearRenderTargets();

	// Meshes
	if (isMeasuring()) m_wrapper->getGPUTimer()->Start(m_profiles[MESH].profile);
	initMeshPass();
	m_meshRenderer->render();
	endMeshPass();
	if (isMeasuring()) m_wrapper->getGPUTimer()->Stop(m_profiles[MESH].profile);

	// Lights
	if (isMeasuring()) m_wrapper->getGPUTimer()->Start(m_profiles[LIGHT].profile);
	initLightPass();
	m_lightRenderSystem->render();
	endLightPass();
	if (isMeasuring()) m_wrapper->getGPUTimer()->Stop(m_profiles[LIGHT].profile);

	if( m_enableEffects ) {
		if (isMeasuring()) m_wrapper->getGPUTimer()->Start(m_profiles[EFFECTS].profile);
		beginEffects();
		m_wrapper->generateEffects();
		endEffects();
		if (isMeasuring()) m_wrapper->getGPUTimer()->Stop(m_profiles[EFFECTS].profile);

		// DoF generation pass
		if (isMeasuring()) m_wrapper->getGPUTimer()->Start(m_profiles[LOW_RES].profile);
		beginGenerateLowRes();
		m_wrapper->generateLowRes();
		endGenerateLowRes();
		if (isMeasuring()) m_wrapper->getGPUTimer()->Stop(m_profiles[LOW_RES].profile);
	}

	//Compose
	if (isMeasuring()) m_wrapper->getGPUTimer()->Start(m_profiles[COMPOSE].profile);
	initComposePass();
	m_wrapper->renderComposeStage();
	endComposePass();
	if (isMeasuring()) m_wrapper->getGPUTimer()->Stop(m_profiles[COMPOSE].profile);

	//Particles
	if (isMeasuring()) m_wrapper->getGPUTimer()->Start(m_profiles[PARTICLE].profile);
	initParticlePass();
	renderParticles();
	endParticlePass();
	if (isMeasuring()) m_wrapper->getGPUTimer()->Stop(m_profiles[PARTICLE].profile);

	//GUI
	if (isMeasuring()) m_wrapper->getGPUTimer()->Start(m_profiles[GUI].profile);
	initGUIPass();
	m_libRocketRenderSystem->render();
	m_antTweakBarSystem->render();
	endGUIPass();
	if (isMeasuring()) m_wrapper->getGPUTimer()->Stop(m_profiles[GUI].profile);

	flipBackbuffer();

	if (isMeasuring()) updateTimers();
}

void GraphicsRendererSystem::initShadowPass(){
	m_wrapper->setRasterizerStateSettings( RasterizerState::FILLED_CW_FRONTCULL );
	m_wrapper->setBlendStateSettings( BlendState::DEFAULT );
	//m_wrapper->setPrimitiveTopology(PrimitiveTopology::TRIANGLELIST);
	m_wrapper->setViewportToShadowMapSize();
	m_wrapper->setRenderingShadows();
	m_wrapper->setShadowViewProjections(m_shadowViewProjections);
}

void GraphicsRendererSystem::endShadowPass(){
	m_wrapper->resetViewportToStdSize();
	m_wrapper->stopedRenderingShadows();
	//m_wrapper->unmapPerShadowBuffer();
}

void GraphicsRendererSystem::initMeshPass(){
	m_wrapper->mapSceneInfo();
	m_wrapper->setRasterizerStateSettings( RasterizerState::DEFAULT );
	m_wrapper->setBlendStateSettings( BlendState::DEFAULT );
	//m_wrapper->setPrimitiveTopology(PrimitiveTopology::TRIANGLELIST);
	m_wrapper->setBaseRenderTargets();
	m_wrapper->setPrimitiveTopology( PrimitiveTopology::TRIANGLELIST );

}

void GraphicsRendererSystem::endMeshPass(){
}

void GraphicsRendererSystem::initLightPass(){
	m_wrapper->setRasterizerStateSettings( RasterizerState::FILLED_CW_FRONTCULL, false );
	m_wrapper->setBlendStateSettings( BlendState::LIGHT );
	m_wrapper->setLightRenderTargets();
	m_wrapper->mapDepthAndNormal();
	//m_wrapper->mapShadows( m_activeShadows );
	m_wrapper->setPrimitiveTopology( PrimitiveTopology::TRIANGLELIST );

}

void GraphicsRendererSystem::endLightPass(){
}

void GraphicsRendererSystem::beginEffects()
{
	Entity* ship = m_world->getEntityManager()->
		getFirstEntityByComponentType(ComponentType::TAG_ShipEditMode);

	if(ship != NULL){
		m_wrapper->isInEditMode(true);
	}
	else{
		m_wrapper->isInEditMode(false);
	}
	m_wrapper->setPrimitiveTopology( PrimitiveTopology::TRIANGLESTRIP );
	m_wrapper->setBlendStateSettings( BlendState::SSAO );
	m_wrapper->setRasterizerStateSettings( RasterizerState::FILLED_NOCULL_NOCLIP, false );
}

void GraphicsRendererSystem::endEffects()
{
	m_wrapper->unmapDepthAndNormal();
	//m_wrapper->unmapShadows(m_activeShadows);

}

void GraphicsRendererSystem::beginGenerateLowRes()
{
	m_wrapper->setViewportToLowRes();
	m_wrapper->setBlendStateSettings( BlendState::OVERWRITE );
	m_wrapper->setDofRenderTargets();
	//m_wrapper->setComposedRenderTargetWithNoDepthStencil();
	m_wrapper->mapGbuffers();
}

void GraphicsRendererSystem::endGenerateLowRes()
{
	m_wrapper->unmapGbuffers();
	m_wrapper->resetViewportToStdSize();

}

void GraphicsRendererSystem::initComposePass()
{
	m_wrapper->setPrimitiveTopology( PrimitiveTopology::TRIANGLESTRIP );
	m_wrapper->setRasterizerStateSettings( RasterizerState::DEFAULT, false );
	m_wrapper->setComposedRenderTargetWithNoDepthStencil();
	m_wrapper->mapGbuffers();
	m_wrapper->mapDofBuffers();
	m_wrapper->mapDepth();
}

void GraphicsRendererSystem::endComposePass()
{
	//m_wrapper->unmapVariousStagesAfterCompose();
	m_wrapper->unmapDofBuffers();
	m_wrapper->unmapGbuffers();
	m_wrapper->unmapDepth();
}

void GraphicsRendererSystem::initParticlePass(){
	m_wrapper->setParticleRenderState();
	//m_wrapper->setBlendStateSettings(BlendState::PARTICLE);
	m_wrapper->setBlendStateSettings( BlendState::ADDITIVE );
	m_wrapper->setPrimitiveTopology( PrimitiveTopology::POINTLIST );
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
	m_wrapper->setComposedRenderTargetWithNoDepthStencil();
}

void GraphicsRendererSystem::initGUIPass(){
	m_wrapper->setPrimitiveTopology( PrimitiveTopology::TRIANGLELIST );
	m_wrapper->setBlendStateSettings( BlendState::ALPHA );
}

void GraphicsRendererSystem::endGUIPass(){
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
	double total = 0;
	GPUTimer* timer = m_wrapper->getGPUTimer();

	for(unsigned int i = 0; i < NUMRENDERINGPASSES-1; i++){
		m_profiles[i].pushNewTime(timer->getTheTimeAndReset(m_profiles[i].profile));
		total += m_profiles[i].renderingTime;
	}
	m_profiles[TOTAL].pushNewTime(total);
	m_wrapper->getGPUTimer()->tick();
}

void GraphicsRendererSystem::enableMeasurement()
{
	m_measureGPU=true;
}

void GraphicsRendererSystem::disableMeasurement()
{
	m_measureGPU=false;
}

bool GraphicsRendererSystem::isMeasuring()
{
	return m_measureGPU;
}

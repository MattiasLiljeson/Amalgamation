#include "ParticleRenderSystem.h"
#include "GraphicsBackendSystem.h"
#include <AglParticleSystem.h>
#include <vector>
#include <aglvector3.h>
#include <AglVector2.h>
#include <GraphicsWrapper.h>


ParticleRenderSystem::ParticleRenderSystem( GraphicsBackendSystem* p_gfxBackend )
										   : EntitySystem( SystemType::ParticleRenderSystem)
{
	m_gfxBackend = p_gfxBackend;
	addParticleSystem();
}


ParticleRenderSystem::~ParticleRenderSystem()
{
	for(unsigned int i = 0; i < m_particleSystems.size();i++){
		delete m_particleSystems[i];
	}
	m_particleSystems.clear();
}

void ParticleRenderSystem::process(){
	for(unsigned int i = 0; i < m_particleSystems.size();i++){
		m_particleSystems[i]->update(m_world->getDelta(), AglVector3(0,0,0));
		renderParticles(m_particleSystems[i]);
	}

	m_gfxBackend->getGfxWrapper()->finalizeFrame();
}

void ParticleRenderSystem::renderParticles(AglParticleSystem *particleSystem){
	m_gfxBackend->renderAParticleSystem(particleSystem);
}

unsigned int ParticleRenderSystem::addParticleSystem( 
	const AglParticleSystemHeader& p_header ){
	m_particleSystems.push_back(new AglParticleSystem(p_header));

	return m_particleSystems.size()-1;
}
unsigned int ParticleRenderSystem::addParticleSystem()
{
	m_particleSystems.push_back(new AglParticleSystem());
	m_particleSystems[0]->setParticleSize(AglVector2(100,100));
	m_particleSystems[0]->setAlignmentType(AglParticleSystemHeader::OBSERVER);
	return m_particleSystems.size()-1;
}
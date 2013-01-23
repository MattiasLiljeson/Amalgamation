#include "ParticleRenderSystem.h"
#include "GraphicsBackendSystem.h"
#include <AglParticleSystem.h>
#include <vector>
#include <GraphicsWrapper.h>
#include "Transform.h"


ParticleRenderSystem::ParticleRenderSystem( GraphicsBackendSystem* p_gfxBackend )
										   : EntitySystem( SystemType::ParticleRenderSystem){
	m_gfxBackend = p_gfxBackend;
}


ParticleRenderSystem::~ParticleRenderSystem()
{
	for(unsigned int i = 0; i < m_particleSystems.size();i++){
		delete m_particleSystems[i].first;
	}
	m_particleSystems.clear();
}

void ParticleRenderSystem::process()
{
	for(unsigned int i = 0; i < m_particleSystems.size();i++)
	{
		m_particleSystems[i].first->update(m_world->getDelta(), AglVector3(0,0,0));
	}
}

void ParticleRenderSystem::renderParticles(AglParticleSystem *particleSystem){
	m_gfxBackend->renderAParticleSystem(particleSystem);
}

unsigned int ParticleRenderSystem::addParticleSystem( 
	const AglParticleSystemHeader& p_header, int p_index){
	m_particleSystems.push_back(pair<AglParticleSystem*, int>(new AglParticleSystem(p_header), p_index));

	return m_particleSystems.size()-1;
}
AglParticleSystem* ParticleRenderSystem::getParticleSystem(int p_index)
{
	for (unsigned int i = 0; i < m_particleSystems.size(); i++)
	{
		if (m_particleSystems[i].second == p_index)
			return m_particleSystems[i].first;
	}
	return NULL;
}

void ParticleRenderSystem::render()
{
	for (unsigned int i = 0; i < m_particleSystems.size(); i++){
		renderParticles(m_particleSystems[i].first);
	}
	
}
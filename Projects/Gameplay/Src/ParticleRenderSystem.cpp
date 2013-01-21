#include "ParticleRenderSystem.h"
#include "GraphicsBackendSystem.h"
#include <AglParticleSystem.h>
#include <vector>
#include <GraphicsWrapper.h>
#include "Transform.h"


ParticleRenderSystem::ParticleRenderSystem( GraphicsBackendSystem* p_gfxBackend )
										   : EntitySystem( SystemType::ParticleRenderSystem){
	m_gfxBackend = p_gfxBackend;
	//addParticleSystem();
}


ParticleRenderSystem::~ParticleRenderSystem()
{
	for(unsigned int i = 0; i < m_particleSystems.size();i++){
		delete m_particleSystems[i];
	}
	m_particleSystems.clear();
}

void ParticleRenderSystem::process(){

	Entity* ship = m_world->getEntityManager()->getFirstEntityByComponentType(
		ComponentType::ShipFlyController);

	for(unsigned int i = 0; i < m_particleSystems.size();i++){
		m_particleSystems[i]->update(m_world->getDelta(), AglVector3(0,0,0));
		renderParticles(m_particleSystems[i]);

		if(ship){
			Transform* trans = static_cast<Transform*>(ship->getComponent(
				ComponentType::Transform));
			m_particleSystems[i]->setSpawnPoint(trans->getTranslation());
			AglVector3 backward = trans->getForward();
			backward *= -1;
			m_particleSystems[i]->setSpawnDirection(backward);
		}
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
	m_particleSystems[0]->setParticleSize(AglVector2(2,2));
	m_particleSystems[0]->setAlignmentType(AglParticleSystemHeader::OBSERVER);
	m_particleSystems[0]->setSpawnFrequency(200.0f);
	m_particleSystems[0]->setSpawnSpeed(5.0f);
	m_particleSystems[0]->setSpread(0.2f);
	m_particleSystems[0]->setFadeOutStart(1.0f);
	m_particleSystems[0]->setFadeInStop(0.5f);
	m_particleSystems[0]->setParticleAge(2.0f);

	return m_particleSystems.size()-1;
}
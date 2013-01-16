#include "ParticleRenderSystem.h"
#include "GraphicsBackendSystem.h"
#include <AglParticleSystem.h>
#include <vector>
#include <aglvector3.h>


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

//void ParticleRenderSystem::processEntities( const vector<Entity*>& p_entities )
//{
//	if (p_entities.size()>0){
//		for(unsigned int i = 0; i < p_entities.size(); i++){
//			/************************************************************************/
//			/* Get the particle systems from the entity that's containing all the	*/
//			/* the systems.	NOT IMPLEMENTED YET										*/
//			/************************************************************************/
//			vector<AglParticleSystem*> particleSystems;// = 
//				//p_entities[i]->getComponent(ComponentType::ParticleEmitters);
//
//			if (particleSystems.size() > 0){
//				for (unsigned int j = 0; j < particleSystems.size(); j++){
//					if (particleSystems[i]){
//						renderParticles(particleSystems[i]);
//					}
//				}
//			}
//		}
//	}
//}

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
	return m_particleSystems.size()-1;
}

void ParticleRenderSystem::process()
{
	for(unsigned int i = 0; i < m_particleSystems.size();i++){
		m_particleSystems[i]->update(m_world->getDelta(), AglVector3(0,0,0));
		renderParticles(m_particleSystems[i]);
	}
}

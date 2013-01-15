#include "ParticleRenderSystem.h"
#include "GraphicsBackendSystem.h"
#include <AglParticleSystem.h>
#include <vector>


ParticleRenderSystem::ParticleRenderSystem( GraphicsBackendSystem* p_gfxBackend )
										   : EntitySystem( SystemType::ParticleRenderSystem,
										   1, ComponentType::ParticleEmitters)
{
	m_gfxBackend = p_gfxBackend;
}


ParticleRenderSystem::~ParticleRenderSystem()
{
}

void ParticleRenderSystem::processEntities( const vector<Entity*>& p_entities )
{
	if (p_entities.size()>0){
		for(unsigned int i = 0; i < p_entities.size(); i++){
			/************************************************************************/
			/* Get the particle systems from the entity that's containing all the	*/
			/* the systems.	NOT IMPLEMENTED YET										*/
			/************************************************************************/
			vector<AglParticleSystem*> particleSystems;// = 
				//p_entities[i]->getComponent(ComponentType::ParticleEmitters);

			if (particleSystems.size() > 0){
				for (unsigned int j = 0; j < particleSystems.size(); j++){
					if (particleSystems[i]){
						renderParticles(particleSystems[i]);
					}
				}
			}
		}
	}
}

void ParticleRenderSystem::renderParticles(AglParticleSystem *particleSystem)
{
	m_gfxBackend->
}

void ParticleRenderSystem::rebuildVertexBuffer( AglParticleSystem* particleSystem )
{

}

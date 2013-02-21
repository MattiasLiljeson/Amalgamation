#include "DestroyOnParticlesDeathSystem.h"
#include "ParticleSystemsComponent.h"

DestroyOnParticlesDeathSystem::DestroyOnParticlesDeathSystem()  : 
	EntitySystem( SystemType::DestroyOnParticlesDeathSystem, 2,
	ComponentType::ComponentTypeIdx::ParticleSystemsComponent,
	ComponentType::ComponentTypeIdx::DestroyOnParticlesDeath)
{

}

DestroyOnParticlesDeathSystem::~DestroyOnParticlesDeathSystem()
{

}

void DestroyOnParticlesDeathSystem::initialize()
{

}

void DestroyOnParticlesDeathSystem::processEntities( const vector<Entity*>& p_entities )
{
	for( unsigned int i=0; i< p_entities.size(); i++ ) 
	{
		Entity* entity = p_entities[i];
		// only need to fetch particles, the constructor ensures that is has a
		// destroyer component
		ParticleSystemsComponent* particleSystems = static_cast<ParticleSystemsComponent*>(
			entity->getComponent( ComponentType::ParticleSystemsComponent) );
		int deadParticleSystemCount=0;
		int totalParticleCount=particleSystems->getParticleSystemCnt();
		for (unsigned int i=0;i<totalParticleCount;i++)
		{
			AglParticleSystem* particleSystem = particleSystems->getParticleSystemPtr(i);
			if (particleSystem->getHeader().spawnType==AglParticleSystemHeader::CONTINUOUSLY)
			{
				deadParticleSystemCount=0; // can't kill if there is at least one continuous
				break;
			}
			else
			{
				// all particles dead and system is at least older than the particle age
				if (particleSystem->getAge()>particleSystem->getHeader().particleAge
					&& particleSystem->getParticles().size()==0)
				{
					deadParticleSystemCount++;
				}
			}
		}
		// if all dead, delete!
		if (deadParticleSystemCount==totalParticleCount)
		{
			m_world->deleteEntity(entity);
		}

	}
}

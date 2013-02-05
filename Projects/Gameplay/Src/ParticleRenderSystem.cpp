#include "ParticleRenderSystem.h"
#include "GraphicsBackendSystem.h"
#include <AglParticleSystem.h>
#include <vector>
#include <GraphicsWrapper.h>
#include "Transform.h"
#include "ParticleEmitters.h"


ParticleRenderSystem::ParticleRenderSystem( GraphicsBackendSystem* p_gfxBackend )
	: EntitySystem( SystemType::ParticleRenderSystem, 1, ComponentType::ParticleEmitters ){
	m_gfxBackend = p_gfxBackend;
}


ParticleRenderSystem::~ParticleRenderSystem()
{
	for(unsigned int i = 0; i < m_collections.size();i++)
	{
		/*delete m_particleSystems[i].first;*/
	}
	m_collections.clear();
}

void ParticleRenderSystem::processEntities( const vector<Entity*>& p_entities )
{
	// Clear the old particle systems
	m_collections.clear();

	// HACK! 
	// must be set to real camera pos as soon as possible. Sorting doesn't work now!
	AglVector3 cameraPos( 0.0f, 0.0f, 0.0f );

	for( unsigned int i = 0; i<p_entities.size(); i++ )
	{
		ParticleEmitters* particlesComp = static_cast<ParticleEmitters*>(
			p_entities[i]->getComponent( ComponentType::ParticleEmitters ) );
		particlesComp->updateParticleSystems( m_world->getDelta(), cameraPos);

		m_collections.push_back( particlesComp->getCollectionPtr() );
		//m_particleSystems[i].first->update( m_world->getDelta(), AglVector3(0.0f, 0.0f, 0.0f) );
	}
}

//void ParticleRenderSystem::renderParticleSystem( AglParticleSystem *p_particleSystem )
//{
//	m_gfxBackend->renderParticleSystem(p_particleSystem);
//}

//unsigned int ParticleRenderSystem::addParticleSystem( 
//	const AglParticleSystemHeader& p_header, int p_index)
//{
//	m_particleSystems.push_back(pair<AglParticleSystem*, int>(new AglParticleSystem(p_header), p_index));
//
//	return m_particleSystems.size()-1;
//}

//AglParticleSystem* ParticleRenderSystem::getParticleSystem(int p_index)
//{
//	for (unsigned int i = 0; i < m_particleSystems.size(); i++)
//	{
//		if (m_particleSystems[i].second == p_index)
//			return m_particleSystems[i].first;
//	}
//	return NULL;
//}

void ParticleRenderSystem::render()
{
	for( unsigned int collectionIdx=0;
		collectionIdx<m_collections.size();
		collectionIdx++ )
	{
		ParticleSystemCollection* collection = m_collections[collectionIdx];
		for( unsigned int systemIdx=0;
			systemIdx < collection->m_particleSystems.size();
			systemIdx++ )
		{
			AglParticleSystem* system = &collection->m_particleSystems[systemIdx];
			m_gfxBackend->renderParticleSystem( system );
		}
	}
	
}
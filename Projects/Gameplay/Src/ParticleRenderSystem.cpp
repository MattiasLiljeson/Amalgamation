#include "ParticleRenderSystem.h"
#include "GraphicsBackendSystem.h"
#include <AglParticleSystem.h>
#include <vector>
#include <GraphicsWrapper.h>
#include "Transform.h"
#include "ParticleEmitters.h"
#include "..\..\AgileAssets\src\AglParticleSystemHeader.h"


ParticleRenderSystem::ParticleRenderSystem( GraphicsBackendSystem* p_gfxBackend )
	: EntitySystem( SystemType::ParticleRenderSystem, 2,
	ComponentType::ParticleEmitters, ComponentType::Transform )
{
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
		Transform* transform = static_cast<Transform*>(
			p_entities[i]->getComponent( ComponentType::Transform ) );

		ParticleEmitters* particlesComp = static_cast<ParticleEmitters*>(
			p_entities[i]->getComponent( ComponentType::ParticleEmitters ) );

		particlesComp->updateParticleSystems( m_world->getDelta(), cameraPos);
		
		// Update only nonrelative particle systems (PS) as the PS's otherwise will get a 
		// double transform
		ParticleSystemCollection* collection = particlesComp->getCollectionPtr();
		for( unsigned int i=0; i<collection->m_collection.size(); i++ ) {
			AglParticleSystemHeader header = collection->m_collection[i].particleSystem.getHeader();
			if( header.space == AglParticleSystemHeader::AglSpace_LOCAL ) {
				particlesComp->setSpawn( transform->getTranslation(), transform->getForward() );
			}
		}
		pair< ParticleSystemCollection*, Transform* > ps( collection, transform );
		m_collections.push_back( ps );
		//m_particleSystems[i].first->update( m_world->getDelta(), AglVector3(0.0f, 0.0f, 0.0f) );
	}
}

void ParticleRenderSystem::render()
{
	for( unsigned int collectionIdx=0;
		collectionIdx<m_collections.size();
		collectionIdx++ )
	{
		ParticleSystemCollection* collection = m_collections[collectionIdx].first;
		Transform* transform = m_collections[collectionIdx].second;
		
		for( unsigned int systemIdx=0;
			systemIdx < collection->m_collection.size();
			systemIdx++ )
		{
			AglParticleSystem* system = &collection->m_collection[systemIdx].particleSystem;
			m_gfxBackend->renderParticleSystem( system, transform->getInstanceDataRef() );
		}
	}
	
}
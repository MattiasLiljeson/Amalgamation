#include "ParticleRenderSystem.h"
#include "GraphicsBackendSystem.h"
#include <AglParticleSystem.h>
#include <vector>
#include <GraphicsWrapper.h>
#include "Transform.h"
#include "ParticleSystemsComponent.h"
#include <AglParticleSystemHeader.h>
#include <ParticleSystemAndTexture.h>

ParticleRenderSystem::ParticleRenderSystem( GraphicsBackendSystem* p_gfxBackend )
	: EntitySystem( SystemType::ParticleRenderSystem, 2,
	ComponentType::ParticleSystemsComponent, ComponentType::Transform )
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

		ParticleSystemsComponent* particlesComp = static_cast<ParticleSystemsComponent*>(
			p_entities[i]->getComponent( ComponentType::ParticleSystemsComponent ) );

		particlesComp->updateParticleSystems( m_world->getDelta(), cameraPos);
		
		if( particlesComp->getParticleSystemCnt() > 0 )
		{
			for( unsigned int i=0; i<particlesComp->getParticleSystemsPtr()->size(); i++ )
			{
				ParticleSystemAndTexture* psAndTex = particlesComp->getParticleSystemAndTexturePtr(i);
				if( psAndTex != NULL ) {
					AglParticleSystemHeader header = psAndTex->particleSystem.getHeader();

					// Update only nonrelative particle systems (PS) as the PS's otherwise will get a 
					// double transform
					if( header.space == AglParticleSystemHeader::AglSpace_LOCAL ) {
						particlesComp->setSpawn( transform->getTranslation(), transform->getForward() );
					}
					pair< ParticleSystemAndTexture*, Transform* > ps( psAndTex, transform );
					m_collections.push_back( ps );
				}
			}
		}
		//m_particleSystems[i].first->update( m_world->getDelta(), AglVector3(0.0f, 0.0f, 0.0f) );
	}
}

void ParticleRenderSystem::render()
{
	for( unsigned int collectionIdx=0;
		collectionIdx<m_collections.size();
		collectionIdx++ )
	{
		ParticleSystemAndTexture* psAndTex = m_collections[collectionIdx].first;
		Transform* transform = m_collections[collectionIdx].second;
		m_gfxBackend->renderParticleSystem( psAndTex, transform->getInstanceDataRef() );
	}
	
}
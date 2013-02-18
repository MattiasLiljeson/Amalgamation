#include "ParticleRenderSystem.h"
#include "GraphicsBackendSystem.h"
#include <AglParticleSystem.h>
#include <vector>
#include <GraphicsWrapper.h>
#include "Transform.h"
#include "ParticleSystemsComponent.h"
#include <AglParticleSystemHeader.h>
#include <ParticleSystemAndTexture.h>
#include "MeshOffsetTransform.h"

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

	// get camera pos, for sorting
	AglVector3 cameraPos( 0.0f, 0.0f, 0.0f );
	auto entitymanager = m_world->getEntityManager();
	Entity* cam = entitymanager->getFirstEntityByComponentType(ComponentType::TAG_MainCamera);
	if (cam)
	{
		Transform* camTransform = static_cast<Transform*>(
			cam->getComponent( ComponentType::Transform ) );
		if (camTransform) cameraPos = camTransform->getTranslation();
	}

	
	// all particle systems
	for( unsigned int i = 0; i<p_entities.size(); i++ )
	{
		Transform* transform = static_cast<Transform*>(
			p_entities[i]->getComponent( ComponentType::Transform ) );

		MeshOffsetTransform* offset = static_cast<MeshOffsetTransform*>(
			p_entities[i]->getComponent( ComponentType::MeshOffsetTransform ) );

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

					AglMatrix transMat = transform->getMatrix();

					// Offset agl-loaded meshes by their offset matrix
					if( offset != NULL ) {
						transMat = offset->offset.inverse() * transMat;
					}

					// Update only local particle systems (PS) as the PS's otherwise will get a 
					// double transform
					if( header.spawnSpace == AglParticleSystemHeader::AglSpace_LOCAL ) {
						particlesComp->setSpawn( transMat );
					}

					pair< ParticleSystemAndTexture*, Transform* > ps( psAndTex, transform );
					m_collections.push_back( ps );
				}
			}
		}
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
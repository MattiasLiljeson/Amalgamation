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
	m_additiveCollection.clear();
	m_alphaCollection.clear();
	m_multiplyCollection.clear();
}

void ParticleRenderSystem::processEntities( const vector<Entity*>& p_entities )
{
	// Clear the old particle systems
	m_additiveCollection.clear();
	m_alphaCollection.clear();
	m_multiplyCollection.clear();


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
					// HACK: always transform spawn until support has been added to the editor. /ML 
					if( header.space == AglParticleSystemHeader::AglSpace_SPAWN_LOCAL || true ) {
						particlesComp->setSpawn( transMat );
					}

					pair< ParticleSystemAndTexture*, Transform* > ps( psAndTex, transform );
					switch( header.modes )
					{
						case AglParticleSystemHeader::AglBlendMode_ADDITIVE:
							m_additiveCollection.push_back( ps );
							break;
						case AglParticleSystemHeader::AglBlendMode_ALPHA:
							m_alphaCollection.push_back( ps );
							break;
						case AglParticleSystemHeader::AglBlendMode_MULTIPLY:
							m_multiplyCollection.push_back( ps );
							break;
						default :
							m_alphaCollection.push_back( ps );
							break;
					}
				}
			}
		}
	}
}

void ParticleRenderSystem::render()
{

}

void ParticleRenderSystem::renderAdditiveParticles()
{
	for( unsigned int collectionIdx=0;
		collectionIdx<m_additiveCollection.size();
		collectionIdx++ )
	{
		ParticleSystemAndTexture* psAndTex = m_additiveCollection[collectionIdx].first;
		Transform* transform = m_additiveCollection[collectionIdx].second;
		m_gfxBackend->renderParticleSystem( psAndTex, transform->getInstanceDataRef() );
	}
}

void ParticleRenderSystem::renderAlphaParticles()
{
	for( unsigned int collectionIdx=0;
		collectionIdx<m_alphaCollection.size();
		collectionIdx++ )
	{
		ParticleSystemAndTexture* psAndTex = m_alphaCollection[collectionIdx].first;
		Transform* transform = m_alphaCollection[collectionIdx].second;
		m_gfxBackend->renderParticleSystem( psAndTex, transform->getInstanceDataRef() );
	}
	
}

void ParticleRenderSystem::renderMultiplyParticles()
{
	for( unsigned int collectionIdx=0;
		collectionIdx<m_multiplyCollection.size();
		collectionIdx++ )
	{
		ParticleSystemAndTexture* psAndTex = m_multiplyCollection[collectionIdx].first;
		Transform* transform = m_multiplyCollection[collectionIdx].second;
		m_gfxBackend->renderParticleSystem( psAndTex, transform->getInstanceDataRef() );
	}
}

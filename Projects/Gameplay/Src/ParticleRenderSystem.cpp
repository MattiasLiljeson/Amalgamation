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

PsRenderInfo::PsRenderInfo( ParticleSystemAndTexture* p_psAndTex, InstanceData p_transform )
{
	psAndTex = p_psAndTex;
	transform = p_transform;
	int tmpBlendMode = psAndTex->particleSystem.getHeader().blendMode;
	blendMode = (AglParticleSystemHeader::AglBlendMode)tmpBlendMode;
	int tmpRastMode = psAndTex->particleSystem.getHeader().rasterizerMode;
	rasterizerMode = (AglParticleSystemHeader::AglRasterizerMode)tmpRastMode;
}


ParticleRenderSystem::ParticleRenderSystem( GraphicsBackendSystem* p_gfxBackend )
	: EntitySystem( SystemType::ParticleRenderSystem, 2,
	ComponentType::ParticleSystemsComponent, ComponentType::Transform )
{
	m_gfxBackend = p_gfxBackend;
}


ParticleRenderSystem::~ParticleRenderSystem()
{
	clearRenderQues();
}

void ParticleRenderSystem::processEntities( const vector<Entity*>& p_entities )
{
	clearRenderQues();


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


		
		if( particlesComp->getParticleSystemCnt() > 0 )
		{
			for( unsigned int i=0; i<particlesComp->getParticleSystemsPtr()->size(); i++ )
			{
				ParticleSystemAndTexture* psAndTex = particlesComp->getParticleSystemAndTexturePtr(i);
				if( psAndTex != NULL ) {
					AglParticleSystemHeader header = psAndTex->particleSystem.getHeader();

					AglMatrix transMat = transform->getMatrix();

					Transform* newTrans = new Transform(transform->getMatrix());


					// Offset Agl-loaded meshes by their offset matrix
					if( offset != NULL ) {
						transMat = offset->offset.inverse() * transMat;
					}
					newTrans->setMatrix(transMat);

					if( header.spawnSpace == AglParticleSystemHeader::AglSpace_LOCAL &&
						header.particleSpace == AglParticleSystemHeader::AglSpace_GLOBAL) 
					{
						particlesComp->setSpawn( transMat, i );
					}

					PsRenderInfo info( psAndTex, newTrans->getInstanceVertex());
					insertToRenderQue( info );
					delete newTrans;
				}
			}
		}
		particlesComp->updateParticleSystems( m_world->getDelta(), cameraPos);

	}
}

void ParticleRenderSystem::render()
{

}

void ParticleRenderSystem::render( AglParticleSystemHeader::AglBlendMode p_blend,
			AglParticleSystemHeader::AglRasterizerMode p_rast )
{
	if( -1<p_blend && p_blend<AglParticleSystemHeader::AglBlendMode_CNT ) {
		if( -1<p_rast && p_rast<AglParticleSystemHeader::AglRasterizerMode_CNT ) {
			for( unsigned int psIdx=0;
				psIdx<m_renderQues[p_blend][p_rast].size();
				psIdx++ )
			{
				ParticleSystemAndTexture* psAndTex = m_renderQues[p_blend][p_rast][psIdx].psAndTex;
				InstanceData transform = m_renderQues[p_blend][p_rast][psIdx].transform;
				m_gfxBackend->renderParticleSystem( psAndTex, transform );
			}
		}
	}
}

BlendState::Mode ParticleRenderSystem::blendStateFromAglBlendMode
	( AglParticleSystemHeader::AglBlendMode p_blend )
{
	switch( p_blend )
	{
	case AglParticleSystemHeader::AglBlendMode_ALPHA:
		return BlendState::PARTICLE;
	case AglParticleSystemHeader::AglBlendMode_ADDITIVE:
		return BlendState::ADDITIVE;
	case AglParticleSystemHeader::AglBlendMode_MULTIPLY:
		return BlendState::MULTIPLY;
	default:
		return BlendState::PARTICLE;
	}
}

RasterizerState::Mode ParticleRenderSystem::rasterizerStateFromAglRasterizerMode(
	AglParticleSystemHeader::AglRasterizerMode p_rast )
{
	switch( p_rast )
	{
	case AglParticleSystemHeader::AglRasterizerMode_Z_CULLED:
		return RasterizerState::DEFAULT;
	case AglParticleSystemHeader::AglRasterizerMode_ALWAYS_ON_TOP:
		return RasterizerState::FILLED_CW_ALWAYSONTOP;
	default:
		return RasterizerState::DEFAULT;
	}
}

bool ParticleRenderSystem::insertToRenderQue( PsRenderInfo p_renderInfo )
{
	int blend = p_renderInfo.blendMode;
	int rast = p_renderInfo.rasterizerMode;
	if( -1<blend && blend<AglParticleSystemHeader::AglBlendMode_CNT ) {
		if( -1<rast && rast<AglParticleSystemHeader::AglRasterizerMode_CNT ) {
			m_renderQues[blend][rast].push_back( p_renderInfo );
			return true;
		}
	}
	return false; 
}

void ParticleRenderSystem::clearRenderQues()
{
	for( int i=0; i<AglParticleSystemHeader::AglBlendMode_CNT; i++ ) {
		for( int j=0; j<AglParticleSystemHeader::AglRasterizerMode_CNT; j++ ) {
			m_renderQues[i][j].clear();
		}
	}
}

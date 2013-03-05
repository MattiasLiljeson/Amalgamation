#include "DamageVisualizerSystem.h"
#include "DamageComponent.h"
#include "ParticleSystemsComponent.h"
#include <ParticleSystemAndTexture.h>


DamageVisualizerSystem::DamageVisualizerSystem( GraphicsBackendSystem* p_gfxBackend )
	: EntitySystem( SystemType::DamageVisualizerSystem, 2,
	ComponentType::ParticleSystemsComponent,
	ComponentType::DamageComponent )
{
	m_gfxBackend = p_gfxBackend;
}


DamageVisualizerSystem::~DamageVisualizerSystem()
{
}

void DamageVisualizerSystem::processEntities( const vector<Entity*>& p_entities )
{
	for( unsigned int i=0; i<p_entities.size(); i++ )
	{
		DamageComponent* dmgComp = static_cast<DamageComponent*>(
			p_entities[i]->getComponent( ComponentType::DamageComponent ) );

		ParticleSystemsComponent* psComp = static_cast<ParticleSystemsComponent*>(
			p_entities[i]->getComponent( ComponentType::ParticleSystemsComponent ) );

		bool skipDrawingThisIteration = false;

		int dmgPsIdx = dmgComp->getDmgParticleSystemIdx();
		if( dmgPsIdx == -1 ) 
		{
			ParticleSystemInstruction instr = createDmgParticleSystemInstruction();
			dmgPsIdx = psComp->addParticleSystemInstruction( instr );
			dmgComp->setDmgParticleSystemIdx( dmgPsIdx );
			skipDrawingThisIteration = true;
		}

		int hitPsIdx = dmgComp->getHitParticleSystemIdx();
		if( hitPsIdx == -1 ) 
		{
			ParticleSystemInstruction instr = createHitParticleSystemInstruction();
			hitPsIdx = psComp->addParticleSystemInstruction( instr );
			dmgComp->setHitParticleSystemIdx( hitPsIdx );
			skipDrawingThisIteration = true;
		}
		
		if( !skipDrawingThisIteration )// It takes one iteration to load and instruction and convert it into a ps.
		{
			float epsilon = 0.01f;

			float dmgOpacity = dmgComp->getDmgBufferPtr()->getLastVal(); //* 1000;// *1000 for debug!
			if( dmgOpacity > epsilon || false)
			{
				ParticleSystemAndTexture* psAndTex = psComp->getParticleSystemAndTexturePtr( dmgPsIdx );
				psAndTex->particleSystem.getHeaderPtr()->maxOpacity = dmgOpacity ;
				psAndTex->particleSystem.restart();
			}

			float hitOpacity = dmgComp->getHitBufferPtr()->getLastVal(); //* 1000;// *1000 for debug!
			if( hitOpacity > epsilon )
			{
				ParticleSystemAndTexture* psAndTex = psComp->getParticleSystemAndTexturePtr( hitPsIdx );
				psAndTex->particleSystem.getHeaderPtr()->maxOpacity = hitOpacity ;
				psAndTex->particleSystem.restart();
			}
		}
	}
}

AglParticleSystemHeader DamageVisualizerSystem::createFullScreenAdditiveQuad()
{
	AglParticleSystemHeader header;
	header.alignmentType = AglParticleSystemHeader::SCREEN;
	header.blendMode = AglParticleSystemHeader::AglBlendMode_ADDITIVE;
	header.fadeInStop = 0.1f;
	header.fadeInStop = 0.0f; // DEBUG
	header.fadeOutStart = 0.1f;
	header.maxOpacity = 1.0f;
	header.particleAge = 0.2;
	header.particleSize = AglVector2( 2.0f, 2.0f );
	header.particleSpace = AglParticleSystemHeader::AglSpace_SCREEN;
	header.rasterizerMode = AglParticleSystemHeader::AglRasterizerMode_ALWAYS_ON_TOP;
	header.spawnType = AglParticleSystemHeader::ONCE;
	return header;
}

ParticleSystemInstruction DamageVisualizerSystem::createDmgParticleSystemInstruction()
{
	ParticleSystemInstruction instr;
	instr.textureFileName = "dmg.png";
	instr.particleSystem = AglParticleSystem( createFullScreenAdditiveQuad() );
	
	return instr;
}

ParticleSystemInstruction DamageVisualizerSystem::createHitParticleSystemInstruction()
{

	ParticleSystemInstruction instr;
	instr.textureFileName = "hit.png";
	AglParticleSystemHeader header = createFullScreenAdditiveQuad();
	header.particleSize = AglVector2( 2.5f, 2.5f );
	header.spawnPoint = AglVector3( 0.0f, -0.3f, 0.0f );
	instr.particleSystem = AglParticleSystem( header );

	return instr;
}

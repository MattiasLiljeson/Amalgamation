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

		int psIdx = dmgComp->getParticleSystemIdx();
		if( psIdx == -1 ) 
		{
			ParticleSystemInstruction instr = createParticleSystemInstruction();
			psIdx = psComp->addParticleSystemInstruction( instr );
			dmgComp->setParticleSystemIdx( psIdx );
		}
		else // It takes one iteration to load and instruction and convert it into a ps.
		{
			float epsilon = 0.01f;
			float opacity = dmgComp->getBufferPtr()->getLastVal() * 1000;

			//opacity = 0.5f; //DEBUG

			if( opacity > epsilon )
			{
				ParticleSystemAndTexture* psAndTex = psComp->getParticleSystemAndTexturePtr( psIdx );
				psAndTex->particleSystem.getHeaderPtr()->maxOpacity = opacity ;
				psAndTex->particleSystem.restart();
			}
		}
	}
}

ParticleSystemInstruction DamageVisualizerSystem::createParticleSystemInstruction()
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

	ParticleSystemInstruction instr;
	instr.textureFileName = "hit.png";
	instr.particleSystem = AglParticleSystem( header );
	
	return instr;
}

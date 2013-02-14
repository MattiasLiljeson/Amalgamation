#include "ParticleSystemInstructionTranslatorSystem.h"
#include "ParticleSystemsComponent.h"
#include <ParticleSystemAndTexture.h>
#include <Globals.h>
#include <GraphicsWrapper.h>
#include "GraphicsBackendSystem.h"


ParticleSystemInstructionTranslatorSystem::ParticleSystemInstructionTranslatorSystem( GraphicsBackendSystem* p_gfxSystem )
	: EntitySystem( SystemType::ParticleSystemInstructionTranslatorSystem, 1,
	ComponentType::ParticleSystemsComponent )
{
	m_gfxSystem = p_gfxSystem;
}


ParticleSystemInstructionTranslatorSystem::~ParticleSystemInstructionTranslatorSystem()
{
}

void ParticleSystemInstructionTranslatorSystem::processEntities( const vector<Entity*>& p_entities )
{
	for( unsigned int entityIdx = 0; entityIdx<p_entities.size(); entityIdx++ )
	{
		ParticleSystemsComponent* particlesComp = static_cast<ParticleSystemsComponent*>(
			p_entities[entityIdx]->getComponent( ComponentType::ParticleSystemsComponent ) );

		if( particlesComp->getInstructionCnt() > 0 )
		{
			vector< pair<ParticleSystemInstruction*, ParticleSystemAndTexture*> >* particleSystems;
			particleSystems = particlesComp->getParticleSystemsPtr();

			for( unsigned int psIdx=0; psIdx<particleSystems->size(); psIdx++ ) 
			{
				ParticleSystemInstruction* instruction = particleSystems->at(psIdx).first;
				if( instruction != NULL ) 
				{
					ParticleSystemAndTexture psAndTexture;
					psAndTexture.particleSystem = instruction->particleSystem;
					int texId = m_gfxSystem->getGfxWrapper()->
						createTexture( instruction->textureFileName, TEXTUREPATH );
					psAndTexture.textureIdx = texId;

					// Remove the instruction BEFORE adding the particle system. 
					// Otherwise The particle system will be treated as a totally new 
					// particle system and the instruction will be moved to the back of 
					// the vector 
					particlesComp->removeParticleSystemInstruction( psIdx );
					particlesComp->addParticleSystem( psAndTexture, psIdx );
				}
			}
		}
	}
}

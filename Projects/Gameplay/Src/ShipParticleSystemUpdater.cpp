#include "ShipParticleSystemUpdater.h"
#include "SpeedBuffer.h"
#include "ParticleSystemsComponent.h"
#include <ParticleSystemAndTexture.h>
#include "ShipInputProcessingSystem.h"
#include <xutility>


ShipParticleSystemUpdater::ShipParticleSystemUpdater()
	: EntitySystem( SystemType::ShipParticleSystemUpdater, 3,
	ComponentType::TAG_MyShip,
	ComponentType::ParticleSystemsComponent,
	ComponentType::SpeedBuffer )
{
}

ShipParticleSystemUpdater::~ShipParticleSystemUpdater()
{
}


void ShipParticleSystemUpdater::initialize()
{

}

void ShipParticleSystemUpdater::processEntities( const vector<Entity*>& p_entities )
{
	if( !p_entities.empty() )
	{
		SpeedBuffer* shipSpeedBuffer = static_cast<SpeedBuffer*>(
			p_entities[0]->getComponent( ComponentType::SpeedBuffer ) );

		ParticleSystemsComponent* shipPs = static_cast<ParticleSystemsComponent*>(
			p_entities[0]->getComponent( ComponentType::ParticleSystemsComponent ) );

		vector< pair<ParticleSystemInstruction*, ParticleSystemAndTexture*> >* pairs =
			shipPs->getParticleSystemsPtr();
		for( unsigned int i=0; i<shipPs->getParticleSystemsPtr()->size(); i++ )
		{
			ParticleSystemAndTexture* psAndTex = (*pairs)[i].second;
			if( psAndTex != NULL )
			{
				AglParticleSystemHeader* header = psAndTex->particleSystem.getHeaderPtr();

				// For smoke
				if( header->spawnSpace == AglParticleSystemHeader::AglSpace_LOCAL )
				{
					//float speedMult = 0.1f;
					float speed = shipSpeedBuffer->m_buffer.getAvg();
        			//speed = max<float>( speed, 1.0f );

					float particlesPerSpawn = psAndTex->psOriginalSettings.particlesPerSpawn;
					float spawnFreq = psAndTex->psOriginalSettings.spawnFrequency;
					float size = psAndTex->psOriginalSettings.particleSize.x;

					float spawnMult = 0.1f;
					header->particlesPerSpawn = particlesPerSpawn * max<float>( 1.0, speed * spawnMult );
					
					float freqMult = 0.25f;
					header->spawnFrequency = spawnFreq * max<float>( 1.0, speed * freqMult );
					
					//float sizeMult = 0.2;
					//header->particleSize.x = size * max<float>( 1.0, speed * sizeMult );
					//header->particleSize.y = size * max<float>( 1.0, speed * sizeMult );
				}

				// For engine fire
				else if( header->particleSpace == AglParticleSystemHeader::AglSpace_LOCAL )
				{
					ShipInputProcessingSystem* input = static_cast<ShipInputProcessingSystem*>(
						m_world->getSystem( SystemType::ShipInputProcessingSystem ) );
					if( input != NULL )
					{
						calculateThrustParticle(input->getProcessedInput().thrustInput,
							psAndTex, header);
					} 
				}
			}
		}

	}
}

void ShipParticleSystemUpdater::calculateThrustParticle(float p_thrust, 
														ParticleSystemAndTexture* p_psAndTex, 
														AglParticleSystemHeader* p_header)
{
	float enginePower = p_thrust;
	float spawnFreq = p_psAndTex->psOriginalSettings.spawnFrequency;
	float particlesPerSpawn = p_psAndTex->psOriginalSettings.particlesPerSpawn;
	float spawnSpeed = p_psAndTex->psOriginalSettings.spawnSpeed;
	float size = p_psAndTex->psOriginalSettings.particleSize.x;

	float freqMult = 5.0f;
	p_header->spawnFrequency = spawnFreq * max<float>( 1.0, enginePower * freqMult );

	float spawnMult = 5.0f;
	p_header->particlesPerSpawn = particlesPerSpawn * max<float>( 1.0, enginePower * spawnMult );

	float sizeMult = 1.5f;
	p_header->particleSize.x = size * max<float>( 1.0, enginePower * sizeMult );
	p_header->particleSize.y = size * max<float>( 1.0, enginePower * sizeMult );

	float speedMult = 1.5f;
	p_header->spawnSpeed = spawnSpeed * max<float>( 1.0, enginePower * speedMult );
}

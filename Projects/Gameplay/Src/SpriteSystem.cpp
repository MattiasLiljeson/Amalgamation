#include "SpriteSystem.h"
#include "ParticleSystemsComponent.h"
#include "Transform.h"

SpriteSystem::SpriteSystem() : 
			  EntitySystem( SystemType::SpriteSystem, 1,
							ComponentType::Sprite)
{
}


SpriteSystem::~SpriteSystem()
{
}

void SpriteSystem::initialize()
{
	/*Entity* effect = m_world->createEntity();

	ParticleSystemsComponent* particleEmitter = new ParticleSystemsComponent();

	effect->addComponent( ComponentType::Transform, new Transform());

	//Flares spreading
	AglParticleSystem flares;
	flares.setSpawnPoint(AglVector3(0.0f, 0.f, 0));
	flares.setSpawnType(AglParticleSystemHeader::ONCE);
	flares.setAlignmentType(AglParticleSystemHeader::SCREEN);
	flares.getHeaderPtr()->blendMode = AglParticleSystemHeader::AglBlendMode_ALPHA;
	flares.setSpawnSpace(AglParticleSystemHeader::AglSpace_SCREEN);
	flares.setParticleSpace(AglParticleSystemHeader::AglSpace_SCREEN);
	flares.setParticleSize(AglVector2(2.0f, 2.0f));
	flares.setParticleAge(100000);

	ParticleSystemInstruction particleInstructionFlares;
	particleInstructionFlares.textureFileName = "hud test.png";
	particleInstructionFlares.particleSystem = flares;
	particleEmitter->addParticleSystemInstruction(particleInstructionFlares);

	effect->addComponent( ComponentType::ParticleSystemsComponent, particleEmitter);
	m_world->addEntity(effect);*/
}

void SpriteSystem::processEntities( const vector<Entity*>& p_entities )
{
}
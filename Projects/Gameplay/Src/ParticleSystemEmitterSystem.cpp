#include "ParticleSystemEmitterSystem.h"

ParticleSystemEmitterSystem::ParticleSystemEmitterSystem()
	: EntitySystem(SystemType::ParticleSystemEmitterSystem, 1, ComponentType::ParticleSystemEmitter_DEPRECATED)
{
}


ParticleSystemEmitterSystem::~ParticleSystemEmitterSystem()
{
}

void ParticleSystemEmitterSystem::initialize()
{
}

void ParticleSystemEmitterSystem::processEntities(const vector<Entity*>& p_entities)
{
	float dt = m_world->getDelta();

	for (unsigned int i = 0; i < p_entities.size(); i++)
	{
	}
}
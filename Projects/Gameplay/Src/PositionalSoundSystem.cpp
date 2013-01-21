#include "PositionalSoundSystem.h"
#include "PositionalSound.h"
#include "AudioBackendSystem.h"


PositionalSoundSystem::PositionalSoundSystem()
	: EntitySystem( SystemType::PositionalSoundSystem, 1, ComponentType::PositionalSound )
{
}

PositionalSoundSystem::~PositionalSoundSystem()
{
}

void PositionalSoundSystem::processEntities( const vector<Entity*>& p_entities )
{
	m_audioBackendSystem = static_cast<AudioBackendSystem*>(m_world->getSystem(
		SystemType::AudioBackendSystem ));
}

void PositionalSoundSystem::initialize()
{

}

void PositionalSoundSystem::added( Entity* p_entity )
{

}

void PositionalSoundSystem::deleted( Entity* p_entity )
{

}

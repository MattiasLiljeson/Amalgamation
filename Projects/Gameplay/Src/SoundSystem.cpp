#include "SoundSystem.h"

SoundSystem::SoundSystem(AudioBackendSystem* p_audioBackend) : 
	EntitySystem(SystemType::SoundSystem, 1, ComponentType::SoundComponent)
{
	m_audioBackend = p_audioBackend;
}

SoundSystem::~SoundSystem()
{

}

void SoundSystem::processEntities( const vector<Entity*>& p_entities )
{

}
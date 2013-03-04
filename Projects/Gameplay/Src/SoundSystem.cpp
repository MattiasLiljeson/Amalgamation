#include "SoundSystem.h"

SoundSystem::SoundSystem() : EntitySystem(SystemType::SoundSystem, 1, 
										  ComponentType::SoundComponent)
{

}

SoundSystem::~SoundSystem()
{

}

void SoundSystem::processEntities( const vector<Entity*>& p_entities )
{

}


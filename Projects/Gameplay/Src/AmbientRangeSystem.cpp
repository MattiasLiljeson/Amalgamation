#include "AmbientRangeSystem.h"

AmbientRangeSystem::AmbientRangeSystem() : EntitySystem( SystemType::AmbientRangeSystem, 
														3, 
														ComponentType::TAG_AmbientRangeSound,
														ComponentType::Transform,
														ComponentType::SoundComponent)
{

}

AmbientRangeSystem::~AmbientRangeSystem()
{

}

void AmbientRangeSystem::processEntities( const vector<Entity*>& p_entities )
{

}

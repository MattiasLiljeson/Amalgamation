#include "AmbientRangeSystem.h"

AmbientRangeSystem::AmbientRangeSystem() : EntitySystem( SystemType::AmbientRangeSystem, 
														1, 
														ComponentType::TAG_AmbientRangeSound )
{

}

AmbientRangeSystem::~AmbientRangeSystem()
{

}

void AmbientRangeSystem::processEntities( const vector<Entity*>& p_entities )
{

}

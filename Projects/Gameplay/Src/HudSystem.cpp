#include "HudSystem.h"


HudSystem::HudSystem() : EntitySystem( SystemType::HudSystem, 1, ComponentType::HudElement )
{
}


HudSystem::~HudSystem()
{
}

void HudSystem::initialize()
{

}

void HudSystem::processEntities( const vector<Entity*>& p_entities )
{

}

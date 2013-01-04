#include "MenuSystem.h"


MenuSystem::MenuSystem()
	: EntitySystem( SystemType::MenuSystem, 1, ComponentType::MenuItem )
{
}


MenuSystem::~MenuSystem()
{
}

void MenuSystem::initialize()
{

}

void MenuSystem::processEntities( const vector<Entity*>& p_entities )
{

}

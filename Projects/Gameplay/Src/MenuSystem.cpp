#include "MenuSystem.h"
#include "LibRocketBackendSystem.h"
#include <Globals.h>
#include <ToString.h>

MenuSystem::MenuSystem()
	: EntitySystem( SystemType::MenuSystem, 1, ComponentType::MenuItem )
{
}


MenuSystem::~MenuSystem()
{
}

void MenuSystem::initialize()
{
	auto rocketBackend = static_cast<LibRocketBackendSystem*>(
		m_world->getSystem(SystemType::LibRocketBackendSystem));

	int docId = rocketBackend->loadDocument(
		(GUI_MENU_PATH + toString("assets/main_menu.rml")).c_str(), true);

	if (docId)
	{
	}
}

void MenuSystem::processEntities( const vector<Entity*>& p_entities )
{

}

#include "MenuSystem.h"
#include "LibRocketBackendSystem.h"
#include <Globals.h>
#include <ToString.h>
#include "MenuItem.h"

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

	rocketBackend->loadDocumentByName("main_menu", true, true);
	rocketBackend->loadDocumentByName("play");
		rocketBackend->loadDocumentByName("join");
		rocketBackend->loadDocumentByName("host");
	rocketBackend->loadDocumentByName("options");
	rocketBackend->loadDocumentByName("credits");

	//if (docId >= 0)
	{
	}
}

void MenuSystem::processEntities( const vector<Entity*>& p_entities )
{

}

void MenuSystem::inserted( Entity* p_entity )
{
	auto menuItem = static_cast<MenuItem*>(p_entity->getComponent(ComponentType::MenuItem));

	auto rocketBackend = static_cast<LibRocketBackendSystem*>(
		m_world->getSystem(SystemType::LibRocketBackendSystem));

	rocketBackend->loadDocumentByName(menuItem->documentName.c_str(), menuItem->initiallyVisible, menuItem->modal);
	for (int i = 0; i < menuItem->handlerNames.size(); i++)
	{
		
	}
}

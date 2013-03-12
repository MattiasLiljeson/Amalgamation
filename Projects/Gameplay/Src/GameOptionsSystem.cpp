#include "GameOptionsSystem.h"
#include <DebugUtil.h>

GameOptionsSystem::GameOptionsSystem()
	: EntitySystem(SystemType::GameOptionsSystem), EventHandler("options")
{
	int i = 0;
}

GameOptionsSystem::~GameOptionsSystem()
{

}

void GameOptionsSystem::processEvent( Rocket::Core::Event& p_event, const Rocket::Core::String& p_value )
{
	// The value here should match the onsubmit attribute in the options-rml-file.
	// For instance, if it says <form onsubmit="apply_settings; goto main_menu"> then it means that
	// the value we are interested in is "apply_settings".
	// However, there could also be other interesting values, such as "restore_settings".
	if (p_value == "apply_settings")
	{
		DEBUGPRINT(("GameOptionsSystem::processEvent(event, value=apply_settings) was called."));
	}
	else if (p_value == "restore_settings")
	{
		DEBUGPRINT(("GameOptionsSystem::processEvent(event, value=restore_settings) was called."));
	}
}

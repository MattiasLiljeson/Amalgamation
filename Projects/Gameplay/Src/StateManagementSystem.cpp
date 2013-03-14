#include "StateManagementSystem.h"
#include "MenuBackgroundSceneSystem.h"
#include "ClientStateSystem.h"

StateManagementSystem::StateManagementSystem()
	: EntitySystem(SystemType::StateManagementSystem)
{

}

StateManagementSystem::~StateManagementSystem()
{

}

void StateManagementSystem::process()
{
	ClientStateSystem* gameState = static_cast<ClientStateSystem*>(
		m_world->getSystem(SystemType::ClientStateSystem));

	if(gameState->getStateDelta(GameStates::MENU) == EnumGameDelta::ENTEREDTHISFRAME){
		// Disable the menu background system. Note currently, that this system will disable
		// itself when the game goes form lobby to loading.
		auto menuBackgroundSys = static_cast<MenuBackgroundSceneSystem*>(
			m_world->getSystem(SystemType::MenuBackgroundSceneSystem));
		menuBackgroundSys->setEnabled(true);
	}
	else if (gameState->getStateDelta(GameStates::LOADING) == EnumGameDelta::ENTEREDTHISFRAME)
	{
		
	}
}

void StateManagementSystem::initialize()
{

}

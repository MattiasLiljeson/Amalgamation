#include "StateManagementSystem.h"
#include "MenuBackgroundSceneSystem.h"
#include "ClientStateSystem.h"
#include "LevelHandlerSystem.h"
#include "ClientModuleCounterSystem.h"
#include "ShipManagerSystem.h"
#include "ConnectionVisualizerSystem.h"
#include "HudSystem.h"
#include "SlotMarkerSystem.h"
#include "ModuleStatusEffectSystem.h"
#include <OutputLogger.h>

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

		if(gameState->getStateDelta(GameStates::LOADING) == EnumGameDelta::EXITTHISFRAME
			|| gameState->getStateDelta(GameStates::FINISHEDLOADING) == EnumGameDelta::EXITTHISFRAME
			|| gameState->getStateDelta(GameStates::INGAME) == EnumGameDelta::EXITTHISFRAME)
		{
			// Cleanup resources here, that hasn't been cleaned up.
			// Currently cleaning up:
			// * Level pieces
			// * Modules
			// * Ships
			// * Connection visualization
			// * Librocket HUD (by hiding it)
			// * Connection visualizer
			// * Slot marker
			// * Module status effects

			auto levelHandler = static_cast<LevelHandlerSystem*>(
				m_world->getSystem(SystemType::LevelHandlerSystem));
			levelHandler->destroyLevel();

			auto moduleCounter = static_cast<ClientModuleCounterSystem*>(
				m_world->getSystem(SystemType::ClientModuleCounterSystem));
			moduleCounter->destroyAllModules();

			auto shipManager = static_cast<ShipManagerSystem*>(
				m_world->getSystem(SystemType::ShipManagerSystem));
			shipManager->destroyAllShips();

			auto hudSystem = static_cast<HudSystem*>(
				m_world->getSystem(SystemType::HudSystem));
			hudSystem->setHUDVisibility(false);

			auto connectionVisualizer = static_cast<ConnectionVisualizerSystem*>(
				m_world->getSystem(SystemType::ConnectionVisualizerSystem));
			connectionVisualizer->cleanup();

			auto slotMarkerSys = static_cast<SlotMarkerSystem*>(
				m_world->getSystem(SystemType::SlotMarkerSystem));
			slotMarkerSys->clear();

			auto moduleStatusEffectSys = static_cast<ModuleStatusEffectSystem*>(
				m_world->getSystem(SystemType::ModuleStatusEffectSystem));
			moduleStatusEffectSys->clear();
			
		}
	}
}

void StateManagementSystem::initialize()
{

}

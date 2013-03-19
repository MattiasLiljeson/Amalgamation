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
#include "PlayerSystem.h"
#include "LobbySystem.h"
#include "ClientConnectToServerSystem.h"
#include <OutputLogger.h>
#include "SelectionMarkerSystem.h"

StateManagementSystem::StateManagementSystem()
	: EntitySystem(SystemType::StateManagementSystem)
{

}

StateManagementSystem::~StateManagementSystem()
{

}

void StateManagementSystem::process()
{
	EntityManager* entityManager = m_world->getEntityManager();

	ClientStateSystem* gameState = static_cast<ClientStateSystem*>(
		m_world->getSystem(SystemType::ClientStateSystem));

	if(gameState->getStateDelta(GameStates::MENU) == EnumGameDelta::ENTEREDTHISFRAME){
		// Disable the menu background system. Note currently, that this system will disable
		// itself when the game goes form lobby to loading.
		auto menuBackgroundSys = static_cast<MenuBackgroundSceneSystem*>(
			m_world->getSystem(SystemType::MenuBackgroundSceneSystem));
		menuBackgroundSys->setEnabled(true);

		if(gameState->getStateDelta(GameStates::LOADING) == EnumGameDelta::EXITTHISFRAME
			|| gameState->getStateDelta(GameStates::LOBBY) == EnumGameDelta::EXITTHISFRAME
			|| gameState->getStateDelta(GameStates::FINISHEDLOADING) == EnumGameDelta::EXITTHISFRAME
			|| gameState->getStateDelta(GameStates::INGAME) == EnumGameDelta::EXITTHISFRAME
			|| gameState->getStateDelta(GameStates::RESULTS) == EnumGameDelta::EXITTHISFRAME)
		{
			// then the following should be done:
			// * Remove all players from the client!
			// * Clear lobby data!
			// * Enable the 'connect to server' system!
			// * Clear components from camera.

			static_cast<PlayerSystem*>(m_world->getSystem(SystemType::PlayerSystem))->
				deleteAllPlayerEntities();
			static_cast<LobbySystem*>(m_world->getSystem(SystemType::LobbySystem))->
				resetAllPlayers();
			static_cast<ClientConnectToServerSystem*>(m_world->getSystem(SystemType::ClientConnectoToServerSystem))->
				setEnabled(true);
			Entity* camera = m_world->getEntityManager()->getFirstEntityByComponentType(ComponentType::TAG_MainCamera);
			if (camera)
			{
				camera->removeComponent(ComponentType::NetworkSynced);
				camera->removeComponent(ComponentType::PlayerState);
				camera->removeComponent(ComponentType::PickComponent);
				camera->removeComponent(ComponentType::PlayerCameraController);
				camera->applyComponentChanges();
			}


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
			hudSystem->clear();

			auto connectionVisualizer = static_cast<ConnectionVisualizerSystem*>(
				m_world->getSystem(SystemType::ConnectionVisualizerSystem));
			connectionVisualizer->cleanup();

			auto slotMarkerSys = static_cast<SlotMarkerSystem*>(
				m_world->getSystem(SystemType::SlotMarkerSystem));
			slotMarkerSys->clear();

			auto moduleStatusEffectSys = static_cast<ModuleStatusEffectSystem*>(
				m_world->getSystem(SystemType::ModuleStatusEffectSystem));
			moduleStatusEffectSys->clear();
			
			auto selectionMarker = static_cast<SelectionMarkerSystem*>(
				m_world->getSystem(SystemType::SelectionMarkerSystem));
			selectionMarker->clear();
		}
	}
}

void StateManagementSystem::initialize()
{

}

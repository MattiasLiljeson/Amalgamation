#include "GameStatsSystem.h"
#include <ComponentType.h>
#include "UpdateClientStatsPacket.h"
#include "DisplayGameStats.h"
#include "PlayerStats.h"

#include <Globals.h>
#include <ToString.h>
#include "libRocketBackendSystem.h"
#include "TimerSystem.h"
#include "InputBackendSystem.h"
#include <Control.h>
#include "ClientStateSystem.h"
#include "PlayerSystem.h"
#include "PlayerComponent.h"
#include "PlayersWinLosePacket.h"
#include "InputActionsBackendSystem.h"

GameStatsSystem::GameStatsSystem()
	: EntitySystem(SystemType::GameStatsSystem)
{
	m_infoPanelVisible = false;
	m_infoPanel = NULL;
	m_rocketDocument = -1;
}
GameStatsSystem::~GameStatsSystem()
{
	delete m_infoPanel;
}

void GameStatsSystem::initialize()
{
	auto rocketBackend = static_cast<LibRocketBackendSystem*>
		(m_world->getSystem(SystemType::LibRocketBackendSystem));


	m_rocketDocument = rocketBackend->loadDocument(
		GUI_HUD_PATH.c_str(), "infoPanel");
	if (m_rocketDocument >= 0)
	{
		rocketBackend->updateElement(m_rocketDocument, "title", "The Scores!");
		m_infoPanel = new DisplayGameStats("playerstats", "infopanel");
	}
}


void GameStatsSystem::updateStats( const UpdateClientStatsPacket* p_packet )
{
	// Check if the infoPanel has the proper size. Otherwise it needs to notify removed or
	// added columns!
	int activePlayersDiff = p_packet->activePlayers - m_infoPanel->getActivePlayers();
	// If diff is a positive value, then players should be added, otherwise removed.
	if (activePlayersDiff != 0)
	{
		if (activePlayersDiff > 0)
		{
			m_infoPanel->addRows(activePlayersDiff);
		} 
		else 
		{
			m_infoPanel->removeRows(-activePlayersDiff);
		}
		m_infoPanel->setActivePlayers(p_packet->activePlayers);
	}

	vector<PlayerComponent*> playerSys  = static_cast<PlayerSystem*>(
		m_world->getSystem(SystemType::PlayerSystem))->getPlayerComponents();

	// Update panel with new data.
	for (int i = 0; i < p_packet->activePlayers; i++)
	{
		PlayerStats stats;
		stats.name	= playerSys.at(i)->m_playerName;
		stats.score = p_packet->scores[i];
		stats.ping	= static_cast<int>(p_packet->ping[i]);

		m_infoPanel->updateRow(i, stats);
	}
}

void GameStatsSystem::updateResults(const PlayersWinLosePacket* p_packet){

	PlayerSystem* system = static_cast<PlayerSystem*>(
		m_world->getSystem(SystemType::PlayerSystem));
	vector<PlayerComponent*> playerSys = system->getPlayerComponents();

	// Update panel with new data.
	for (int i = 0; i < p_packet->activePlayers; i++)
	{
		PlayerStats stats;
		stats.name	= system->getPlayerNameFromID(p_packet->playerIdentities[i]);
		stats.score = p_packet->scores[i];
		stats.ping	= 0;

		m_infoPanel->updateRow(i, stats);
	}
}

void GameStatsSystem::process()
{

	ClientStateSystem* gameState = static_cast<ClientStateSystem*>(
		m_world->getSystem(SystemType::ClientStateSystem));

	if(gameState->getCurrentState() == GameStates::INGAME ){

		InputActionsBackendSystem* inputSystem = static_cast<InputActionsBackendSystem*>(
			m_world->getSystem(SystemType::InputActionsBackendSystem));
	
		LibRocketBackendSystem* rocketBackend = static_cast<LibRocketBackendSystem*>
			(m_world->getSystem(SystemType::LibRocketBackendSystem));

		double delta = inputSystem->getDeltaByAction(InputActionsBackendSystem::Actions_SHOW_SCORE);

		if( delta > 0.5) {
			rocketBackend->showDocument( m_rocketDocument );
			m_infoPanel->updateTheVisualInfoPanel();
		} else if( delta < -0.5f) {
			rocketBackend->hideDocument( m_rocketDocument );
		}
	}

	else if( gameState->getCurrentState() == GameStates::RESULTS){

		auto rocketBackend = static_cast<LibRocketBackendSystem*>
			(m_world->getSystem(SystemType::LibRocketBackendSystem));

		auto actionInputSystem = static_cast<InputActionsBackendSystem*>(
			m_world->getSystem(SystemType::InputActionsBackendSystem));

		if(gameState->getStateDelta(GameStates::RESULTS) == EnumGameDelta::ENTEREDTHISFRAME){
			rocketBackend->updateElement(m_rocketDocument, "title", "The Final Results!");
			rocketBackend->showDocument(m_rocketDocument);
		}

		if(actionInputSystem->getDeltaByAction
			(InputActionsBackendSystem::Actions_THRUST_FORWARD) > 0.5f){
			gameState->setQueuedState(GameStates::MENU);
		}

		//Always show the info panel during the results! 
		m_infoPanelVisible = true;
	}

	if (m_infoPanelVisible)
	{
		auto timerSystem = static_cast<TimerSystem*>(
			m_world->getSystem(SystemType::TimerSystem));

		if (timerSystem->checkTimeInterval(TimerIntervals::EverySecond))
		{
			m_infoPanel->updateTheVisualInfoPanel();
		}
	}
}
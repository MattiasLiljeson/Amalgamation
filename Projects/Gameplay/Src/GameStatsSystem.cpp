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

GameStatsSystem::GameStatsSystem()
	: EntitySystem(SystemType::GameStatsSystem)
{
	m_infoPanelVisible = false;
	m_infoPanel = NULL;
	m_infoPanelDoc = -1;
}
GameStatsSystem::~GameStatsSystem()
{
	delete m_infoPanel;
}

void GameStatsSystem::initialize()
{
	auto rocketBackend = static_cast<LibRocketBackendSystem*>
		(m_world->getSystem(SystemType::LibRocketBackendSystem));


	m_infoPanelDoc = rocketBackend->loadDocument(
		GUI_HUD_PATH.c_str(), "infoPanel");
	if (m_infoPanelDoc >= 0)
	{
		rocketBackend->updateElement(m_infoPanelDoc, "title", "Client Table:");
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

	// Update panel with new data.
	for (int i = 0; i < p_packet->activePlayers; i++)
	{
		PlayerStats stats;
		stats.name	= toString(p_packet->playerIdentities[i]);
		stats.score = p_packet->scores[i];
		stats.ping	= static_cast<int>(p_packet->ping[i]);

		m_infoPanel->updateRow(i, stats);
	}
}

void GameStatsSystem::process()
{
	auto inputSystem = static_cast<InputBackendSystem*>(
		m_world->getSystem(SystemType::InputBackendSystem));
	
	auto rocketBackend = static_cast<LibRocketBackendSystem*>
		(m_world->getSystem(SystemType::LibRocketBackendSystem));

	if ((inputSystem->getControlByEnum(InputHelper::KeyboardKeys_T))->getStatus() > 0.5f)
	{
		if (!m_infoPanelVisible)
		{
			m_infoPanelVisible = !m_infoPanelVisible;
			rocketBackend->showDocument(m_infoPanelDoc);
			m_infoPanel->updateTheVisualInfoPanel();
		}
	}
	else
	{
		if (m_infoPanelVisible)
		{
			m_infoPanelVisible = !m_infoPanelVisible;
			rocketBackend->hideDocument(m_infoPanelDoc);
		}
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
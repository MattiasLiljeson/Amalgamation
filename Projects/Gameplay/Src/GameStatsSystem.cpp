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
	m_displayInfoPanel = false;
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
		(GUI_HUD_PATH + toString("infoPanel.rml")).c_str(), false);

	m_infoPanel = new DisplayGameStats("playerstats");
}


void GameStatsSystem::updateStats( const UpdateClientStatsPacket* p_packet )
{
	for (int i = 0; i < MAXPLAYERS; i++)
	{
		PlayerStats stats;
		stats.name	= "Apa";
		stats.score = p_packet->scores[i];
		stats.ping	= p_packet->ping[i];

		m_infoPanel->updateRow(i, stats);
	}
}

void GameStatsSystem::process()
{
	auto inputSystem = static_cast<InputBackendSystem*>(
		m_world->getSystem(SystemType::InputBackendSystem));
	
	auto rocketBackend = static_cast<LibRocketBackendSystem*>
		(m_world->getSystem(SystemType::LibRocketBackendSystem));

	if ((inputSystem->getControlByEnum(InputHelper::KEY_T))->getStatus() > 0.5f)
	{
		if (!m_displayInfoPanel)
		{
			m_displayInfoPanel = !m_displayInfoPanel;
			rocketBackend->showDocument(m_infoPanelDoc);
		}
	}
	else
	{
		if (m_displayInfoPanel)
		{
			m_displayInfoPanel = !m_displayInfoPanel;
			rocketBackend->hideDocument(m_infoPanelDoc);
		}
	}

	if (m_displayInfoPanel)
	{
		auto timerSystem = static_cast<TimerSystem*>(
			m_world->getSystem(SystemType::TimerSystem));

		if (timerSystem->checkTimeInterval(TimerIntervals::EverySecond)){
			m_infoPanel->updateTheVisualInfoPanel();
		}
	}
}

#include "DisplayPlayerScoreSystem.h"

#include <AntTweakBarWrapper.h>
#include "PlayerScore.h"
#include "NetworkSynced.h"
#include "HudElement.h"
#include "TcpClient.h"
#include <sstream>
#include "LibRocketBackendSystem.h"
#include "TimerSystem.h"
#include <Globals.h>
#include <string>
#include <ToString.h>
#include "HudSystem.h"

DisplayPlayerScoreSystem::DisplayPlayerScoreSystem(TcpClient* p_client)
	: EntitySystem( SystemType::DisplayPlayerScoreSystem,
	2, ComponentType::PlayerScore, ComponentType::NetworkSynced )
{
	m_client		= p_client;
	m_playerEntity	= NULL;
}

DisplayPlayerScoreSystem::~DisplayPlayerScoreSystem()
{
}

void DisplayPlayerScoreSystem::initialize()
{
	//std::string tmp = "gui/temp/tutorial.rml";
	//m_scoreTableDocId = m_librocketBackend->loadDocument(tmp.c_str());
}

void DisplayPlayerScoreSystem::process()
{
	auto timerSys = static_cast<TimerSystem*>(m_world->getSystem(SystemType::TimerSystem));

	if (timerSys->checkTimeInterval(TimerIntervals::EverySecond))
	{
		if (m_playerEntity)
		{
			/*
			// TODO: refactoring... This is maybe some weird shit I write here // Alex
			auto score	= static_cast<PlayerScore*>
				(m_playerEntity->getComponent( ComponentType::PlayerScore ));

			auto hudElementSys = static_cast<HudSystem*>(
				m_world->getSystem(SystemType::HudSystem));

			auto hudEntity = hudElementSys->getActiveEntities()[0];

			if (hudEntity)
			{
				auto hud = static_cast<HudElement*>
					(hudEntity->getComponent( ComponentType::HudElement ));
				hud->setValue("Score:&nbsp;" + toString(score->getTotalScore()));
			}
			*/
		}
	}
}


void DisplayPlayerScoreSystem::inserted( Entity* p_entity )
{
	PlayerScore* score =
		static_cast<PlayerScore*>(p_entity->getComponent( ComponentType::PlayerScore ));
	NetworkSynced* netSync =
		static_cast<NetworkSynced*>(p_entity->getComponent( ComponentType::NetworkSynced ));
	stringstream ss;
	ss << "score(" << netSync->getNetworkOwner() << ")";
	AntTweakBarWrapper::getInstance()->addReadOnlyVariable( AntTweakBarWrapper::OVERALL,
		ss.str().c_str(), TwType::TW_TYPE_FLOAT, score->getScorePointer(), "" );

	if (netSync->getNetworkOwner() == m_client->getId())
	{
		m_playerEntity = p_entity;
	}
}

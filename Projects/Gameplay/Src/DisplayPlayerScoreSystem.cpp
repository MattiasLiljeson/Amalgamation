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

DisplayPlayerScoreSystem::DisplayPlayerScoreSystem(LibRocketBackendSystem* p_librocketBackend,
												   TcpClient* p_client)
	: EntitySystem( SystemType::DisplayPlayerScoreSystem,
	2, ComponentType::PlayerScore, ComponentType::NetworkSynced )
{
	m_client			= p_client;
	m_playerScoreMenu	= NULL;
	m_librocketBackend	= p_librocketBackend;
}

DisplayPlayerScoreSystem::~DisplayPlayerScoreSystem()
{
}

void DisplayPlayerScoreSystem::initialize()
{
	std::string tmp = "gui/temp/tutorial.rml";
	m_scoreTableDocId = m_librocketBackend->loadDocument(tmp.c_str());
}

void DisplayPlayerScoreSystem::processEntities( const vector<Entity*>& p_entities )
{
	auto timerSys = static_cast<TimerSystem*>(m_world->getSystem(SystemType::TimerSystem));

	if (timerSys->checkTimeInterval(TimerIntervals::EverySecond))
	{
		if (m_playerScoreMenu)
		{
			stringstream hudvalue;
			hudvalue << "Client Id , Score , Ping";

			for (int i = 0; i < p_entities.size(); i++)
			{
				Entity* entity = p_entities[i];

				PlayerScore* score =
					static_cast<PlayerScore*>(entity->getComponent( ComponentType::PlayerScore ));
				NetworkSynced* netSync =
					static_cast<NetworkSynced*>(entity->getComponent( ComponentType::NetworkSynced ));

				hudvalue << "\n" << netSync->getNetworkOwner() << " " << score->getTotalScore()
					<< " " << "N/A";
			}


			m_playerScoreMenu->setValue(hudvalue.str());
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
		ss.str().c_str(), TwType::TW_TYPE_FLOAT, score->getTotalScorePointer(), "" );

	// If the entity belongs to the current owner, the stored hud element component can	// be fetched here.
	if (netSync->getNetworkOwner() == m_client->getId())
	{
		m_playerScoreMenu = 
			static_cast<HudElement*>(p_entity->getComponent( ComponentType::HudElement ));
	}
}

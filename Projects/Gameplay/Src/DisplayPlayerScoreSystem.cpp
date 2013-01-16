#include "DisplayPlayerScoreSystem.h"

#include <AntTweakBarWrapper.h>
#include "PlayerScore.h"
#include "NetworkSynced.h"
#include "HudElement.h"
#include "TcpClient.h"
#include <sstream>

DisplayPlayerScoreSystem::DisplayPlayerScoreSystem(TcpClient* p_client)
	: EntitySystem( SystemType::DisplayPlayerScoreSystem,
	2, ComponentType::PlayerScore, ComponentType::NetworkSynced)
{
	m_client			= p_client;
	m_playerScoreMenu	= NULL;
}

DisplayPlayerScoreSystem::~DisplayPlayerScoreSystem()
{
}

void DisplayPlayerScoreSystem::processEntities( const vector<Entity*>& p_entities )
{
	if (m_playerScoreMenu)
	{
		stringstream hudvalue("Client Id , Score , Ping");

		for (int i = 0; i < p_entities.size(); i++)
		{
			Entity* entity = p_entities[i];

			PlayerScore* score =
				static_cast<PlayerScore*>(entity->getComponent( ComponentType::PlayerScore ));
			NetworkSynced* netSync =
				static_cast<NetworkSynced*>(entity->getComponent( ComponentType::NetworkSynced ));

			//hudvalue << "<br/>" << netSync->getNetworkOwner() << " " << score->getScore()
			//	<< " " << "NA";
		}

		m_playerScoreMenu->setValue("hej");
	}
}

void DisplayPlayerScoreSystem::initialize()
{

}


void DisplayPlayerScoreSystem::inserted( Entity* p_entity )
{
	PlayerScore* score =
		static_cast<PlayerScore*>(p_entity->getComponent( ComponentType::PlayerScore ));
	NetworkSynced* netSync =
		static_cast<NetworkSynced*>(p_entity->getComponent( ComponentType::NetworkSynced ));

	stringstream ss;
	ss << "score(" << netSync->getNetworkOwner() << ")";

	AntTweakBarWrapper::getInstance()->addReadOnlyVariable( AntTweakBarWrapper::NETWORK,
		ss.str().c_str(), TwType::TW_TYPE_INT32, score->getScorePointer(), "" );

	// If the entity belongs to the current owner, the stored hud element component can
	// be fetched here.
	if (netSync->getNetworkOwner() == m_client->getId())
	{
		m_playerScoreMenu = 
			static_cast<HudElement*>(p_entity->getComponent( ComponentType::HudElement ));
	}
}

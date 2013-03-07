#include "PlayerSystem.h"
#include "PlayerComponent.h"
#include <Globals.h>
#include <string>
using namespace std;

PlayerSystem::PlayerSystem() : EntitySystem( SystemType::PlayerSystem, 1, 
											ComponentType::PlayerComponent)
{
	m_playerComponents.resize(MAXPLAYERS, NULL);
}

PlayerSystem::~PlayerSystem()
{

}

PlayerComponent* PlayerSystem::getPlayerCompFromNetworkComp(NetworkSynced* p_netComponent)
{


	vector<Entity*> activeEntities = getActiveEntities();

	Entity* player = activeEntities[p_netComponent->getPlayerID()];

	return static_cast<PlayerComponent*>
		(player->getComponent(ComponentType::PlayerComponent));
}

const vector<PlayerComponent*>& PlayerSystem::getPlayerComponents() const
{
	return m_playerComponents;
}

void PlayerSystem::inserted( Entity* p_entity )
{
	PlayerComponent* playerComponent = static_cast<PlayerComponent*>
		(p_entity->getComponent(ComponentType::PlayerComponent));

	if(playerComponent)
		m_playerComponents.push_back(playerComponent);
}

std::string PlayerSystem::getPlayerNameFromID( int p_playerID )
{
	if(p_playerID <= m_playerComponents.size()){
		for (unsigned int i = 0; i < m_playerComponents.size(); i++)
		{
			if(m_playerComponents.at(i)->m_playerID == p_playerID){
				return m_playerComponents.at(i)->m_playerName;
			}
		}
	}
	//if (m_playerComps.hasValue(p_playerID))
	//	return m_playerComps[p_playerID]->m_playerName;
	return "";
}

void PlayerSystem::removed( Entity* p_entity )
{
	auto playerComponent = static_cast<PlayerComponent*>(
		p_entity->getComponent(ComponentType::PlayerComponent) );

	// Recycles the id. Note, this is only useful on the server.
	recyclePlayerId(playerComponent->m_playerID);
}
//
int PlayerSystem::createPlayerId( int p_fromNetworkOwnerId )
{
	return m_playerIds.add(p_fromNetworkOwnerId);
}

void PlayerSystem::recyclePlayerId( int p_playerId )
{
	if (m_playerIds.hasValue(p_playerId))
		m_playerIds.removeAt(p_playerId);
}

//PlayerComponent* PlayerSystem::serverCreatePlayerComponent()
//{
//	PlayerComponent* playerComp = new PlayerComponent();
//	playerComp->m_playerID		= m_playerComps.add(playerComp);
//	return playerComp;
//}

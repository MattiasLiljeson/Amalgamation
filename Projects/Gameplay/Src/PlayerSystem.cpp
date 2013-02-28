#include "PlayerSystem.h"
#include "PlayerComponent.h"

PlayerSystem::PlayerSystem() : EntitySystem( SystemType::PlayerSystem, 1, 
											ComponentType::PlayerComponent)
{

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

std::string PlayerSystem::getPlayerNameFromID( int p_playerID ) const
{
	if(p_playerID <= m_playerComponents.size()){
		for (unsigned int i = 0; i < m_playerComponents.size(); i++)
		{
			if(m_playerComponents.at(i)->m_playerID == p_playerID){
				return m_playerComponents.at(i)->m_playerName;
			}
		}
	}
	return "";
}

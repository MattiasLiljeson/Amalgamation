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

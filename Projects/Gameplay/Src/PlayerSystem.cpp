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

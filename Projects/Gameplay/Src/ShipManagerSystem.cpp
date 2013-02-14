#include "ShipManagerSystem.h"
#include "NetworkSynced.h"

ShipManagerSystem::ShipManagerSystem() : 
	EntitySystem( SystemType::ShipManagerSystem, 1,
	ComponentType::TAG_Ship)
{
}

Entity* ShipManagerSystem::findShip(int p_ownerId)
{
	for (unsigned int i = 0; i < getActiveEntities().size(); i++)
	{
		NetworkSynced* netSync = static_cast<NetworkSynced*>(getActiveEntities()[i]->getComponent(ComponentType::NetworkSynced));
		if (netSync->getNetworkOwner() == p_ownerId)
			return getActiveEntities()[i];
	}
	return NULL;
}

const vector<Entity*>& ShipManagerSystem::getShips() const
{
	return getActiveEntities();
}
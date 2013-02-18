#include "ShipManagerSystem.h"
#include "NetworkSynced.h"
#include "ConnectionPointSet.h"

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
vector<FreeSlotData> ShipManagerSystem::findFreeConnectionPoints(int p_ownerId)
{
	Entity* ship = findShip(p_ownerId);

	ConnectionPointSet* cps = static_cast<ConnectionPointSet*>(ship->getComponent(ComponentType::ConnectionPointSet));
	
	vector<FreeSlotData> conPoints;
	for (unsigned int i = 0; i < cps->m_connectionPoints.size(); i++)
	{
		if (cps->m_connectionPoints[i].cpConnectedEntity == -1)
		{
			FreeSlotData data;
			data.index = i;
			data.parent = ship;
			data.offset = cps->m_connectionPoints[i].cpTransform;
			conPoints.push_back(data);
		}
	}
	return conPoints;
}
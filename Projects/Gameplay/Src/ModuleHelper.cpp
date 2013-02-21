#include "ModuleHelper.h"

#include "ShipModule.h"
#include "NetworkSynced.h"

void ModuleHelper::FindParentShip( EntityWorld* p_world,
								  Entity** p_inoutShip, ShipModule** p_inoutModule )
{
	ShipModule* curr = *p_inoutModule;
	while (curr && curr->m_parentEntity > -1)
	{
		*p_inoutShip = p_world->getEntity((*p_inoutModule)->m_parentEntity);
		curr = static_cast<ShipModule*>((*p_inoutShip)->getComponent(
			ComponentType::ShipModule));
	}
}

int ModuleHelper::FindParentShipClientId( EntityWorld* p_world, ShipModule** p_inoutModule )
{
	Entity* ship = NULL;
	ModuleHelper::FindParentShip(p_world,&ship,p_inoutModule);
	int ownerId = -1;

	if (ship)
	{
		auto netsynccomp = p_world->getComponentManager()->getComponent(ship,
			ComponentType::NetworkSynced);
		if (netsynccomp)
		{
			auto netsync = static_cast<NetworkSynced*>(netsynccomp);
			if (netsync)
				ownerId = netsync->getNetworkOwner();
		}
	}

	return ownerId;
}
#include "ModuleHelper.h"

#include "ShipModule.h"
#include "NetworkSynced.h"

void ModuleHelper::FindParentShip( EntityWorld* p_world,
								   Entity** p_inoutShip, ShipModule* p_module )
{
	ShipModule* module = p_module;
	while (module && module->m_parentEntity > -1)
	{
		*p_inoutShip = p_world->getEntity(module->m_parentEntity);
		module = static_cast<ShipModule*>((*p_inoutShip)->getComponent(
			ComponentType::ShipModule));
	}
}

int ModuleHelper::FindParentShipClientId( EntityWorld* p_world, ShipModule* p_module, 
										  Entity** p_outShipEntity )
{
	Entity* ship = NULL;
	ModuleHelper::FindParentShip(p_world,&ship,p_module);
	if (p_outShipEntity) *p_outShipEntity = ship;
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
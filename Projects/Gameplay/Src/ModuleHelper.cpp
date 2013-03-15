#include "ModuleHelper.h"

#include "ShipModule.h"
#include "NetworkSynced.h"
#include "PlayerSystem.h"

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

float ModuleHelper::changeModuleValueOnDetach( float p_value )
{
	return p_value*0.5f;
}
void ModuleHelper::FindScoreComponent(EntityWorld* p_world, ShipModule* p_module, PlayerComponent** p_score)
{
	Entity* ship;
	FindParentShip(p_world, &ship, p_module);
	PlayerSystem* playerSys = static_cast<PlayerSystem*>
		(p_world->getSystem(SystemType::PlayerSystem));
	NetworkSynced* sync = static_cast<NetworkSynced*>(ship->getComponent(ComponentType::NetworkSynced));
	(*p_score) = playerSys->getPlayerCompFromNetworkComp(sync);
}
void ModuleHelper::FindScoreComponent(EntityWorld* p_world, Entity* p_ship, PlayerComponent** p_score)
{
	PlayerSystem* playerSys = static_cast<PlayerSystem*>
		(p_world->getSystem(SystemType::PlayerSystem));
	NetworkSynced* sync = static_cast<NetworkSynced*>(p_ship->getComponent(ComponentType::NetworkSynced));
	(*p_score) = playerSys->getPlayerCompFromNetworkComp(sync);
}
void ModuleHelper::FindScoreComponent(EntityWorld* p_world, int p_clientID, PlayerComponent** p_score)
{
	PlayerSystem* playerSys = static_cast<PlayerSystem*>
		(p_world->getSystem(SystemType::PlayerSystem));
	(*p_score) = playerSys->findPlayerComponentFromNetworkID(p_clientID);
}

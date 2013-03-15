#pragma once
#include <Entity.h>

class ShipModule;
class PlayerComponent;

// =======================================================================================
//                                      ModuleHelper
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Helper class for modules
///        
/// # ModuleHelper
/// Detailed description.....
/// Created on: 14-2-2013 
///---------------------------------------------------------------------------------------

class ModuleHelper
{
public:
	static void FindParentShip(EntityWorld* p_world,
		Entity** p_inoutShip, ShipModule* p_module);
	static int FindParentShipClientId(EntityWorld* p_world, 
		ShipModule* p_module,Entity** p_outShipEntity=NULL);
	static float changeModuleValueOnDetach(float p_value);

	static void FindScoreComponent(EntityWorld* p_world, ShipModule* p_module, PlayerComponent** p_score);
	static void FindScoreComponent(EntityWorld* p_world, Entity* p_ship, PlayerComponent** p_score);
	static void FindScoreComponent(EntityWorld* p_world, int p_clientID, PlayerComponent** p_score);
protected:
private:
};
#pragma once
#include <Entity.h>

class ShipModule;

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
	static int FindParentShipClientId(EntityWorld* p_world, ShipModule* p_module,Entity** p_outShipEntity=NULL);
protected:
private:
};
#pragma once
#include <EntitySystem.h>
#include "ShipModule.h"
// =======================================================================================
// ShipModulesTrackerSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief...
///        
/// # ShipModulesTrackerSystem
/// Detailed description...
/// Created on: 31-1-2013 
///---------------------------------------------------------------------------------------
class ShipModulesTrackerSystem: public EntitySystem
{
public:
	ShipModulesTrackerSystem();
	vector<ShipModule*> getModulesFromParent( int p_entityIndex );

	void destroyAllModules();
};
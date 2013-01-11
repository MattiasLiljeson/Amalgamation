#pragma once

#include <EntitySystem.h>
#include "AglVector3.h"
#include "ConnectionPointSet.h"

// =======================================================================================
//                                      ShipModulesControllerSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	System for handling communication with the physics library
///        
/// # ShipModulesControllerSystem
/// Detailed description.....
/// Created on: 11-12-2012 
///---------------------------------------------------------------------------------------
class ShipModulesControllerSystem: public EntitySystem
{
public:
	ShipModulesControllerSystem();
	~ShipModulesControllerSystem();

	virtual void initialize();
	void processEntities(const vector<Entity*>& p_entities );

	void applyImpulse(int p_bodyID, AglVector3 p_impulse, AglVector3 p_angularImpulse);

};
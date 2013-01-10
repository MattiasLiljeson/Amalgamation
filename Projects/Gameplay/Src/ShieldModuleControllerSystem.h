#pragma once

#include <EntitySystem.h>
#include "ShieldModule.h"

// =======================================================================================
//                                      PhysicsSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	System for handling communication with the physics library
///        
/// # PhysicsSystem
/// Detailed description.....
/// Created on: 11-12-2012 
///---------------------------------------------------------------------------------------
class ShieldModuleControllerSystem: public EntitySystem
{
public:
	ShieldModuleControllerSystem();
	~ShieldModuleControllerSystem();

	virtual void initialize();
	void processEntities(const vector<Entity*>& p_entities );
};
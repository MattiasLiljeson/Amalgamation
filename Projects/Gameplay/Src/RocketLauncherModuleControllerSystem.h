#pragma once

#include <EntitySystem.h>
#include "RocketLauncherModule.h"
#include "Transform.h"

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
class RocketLauncherModuleControllerSystem: public EntitySystem
{
public:
	RocketLauncherModuleControllerSystem();
	~RocketLauncherModuleControllerSystem();

	virtual void initialize();
	void processEntities(const vector<Entity*>& p_entities );
private:
	void handleLaserSight(Entity* p_entity);
	void spawnRocket(Entity* p_entity);
};
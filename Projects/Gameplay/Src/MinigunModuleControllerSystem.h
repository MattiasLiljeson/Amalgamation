#pragma once

#include <EntitySystem.h>
#include "MinigunModule.h"

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
class MinigunModuleControllerSystem: public EntitySystem
{
public:
	MinigunModuleControllerSystem();
	~MinigunModuleControllerSystem();

	virtual void initialize();
	void processEntities(const vector<Entity*>& p_entities );
private:
	void handleLaserSight(Entity* p_entity);
	void spawnBullet(Entity* p_entity);
};
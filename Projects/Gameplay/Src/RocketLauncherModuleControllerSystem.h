#pragma once

#include <EntitySystem.h>
#include "RocketLauncherModule.h"
#include "Transform.h"
#include "TcpServer.h"
#include <AglVector3.h>

class ShipModule;

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
	RocketLauncherModuleControllerSystem(TcpServer* p_server);
	~RocketLauncherModuleControllerSystem();

	virtual void initialize();
	void processEntities(const vector<Entity*>& p_entities );
private:
	void handleLaserSight(Entity* p_entity);
	void spawnRocket(Entity* p_entity,ShipModule* p_module);
	Entity* getClosestShip(Entity* p_entity, Entity* p_parentShip, AglVector3& p_target);
private:
	TcpServer* m_server;
};


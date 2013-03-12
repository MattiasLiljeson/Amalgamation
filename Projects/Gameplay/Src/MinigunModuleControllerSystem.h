#pragma once

#include <EntitySystem.h>
#include "MinigunModule.h"
#include "TcpServer.h"

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
	MinigunModuleControllerSystem(TcpServer* p_server);
	~MinigunModuleControllerSystem();

	virtual void initialize();
	void processEntities(const vector<Entity*>& p_entities );
private:
	void handleLaserSight(Entity* p_entity, AglVector3 p_pos, AglVector3 p_dir);
	void startAnimation(Entity* p_gun);
	void stopAnimation(Entity* p_gun);
	void updateRay(Entity* p_gun, AglVector3& p_rayPos, AglVector3& p_rayDir);

private:
	TcpServer* m_server;
};
#pragma once

#include <EntitySystem.h>
#include "MineLayerModule.h"
#include "Transform.h"
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
class MineLayerModuleControllerSystem: public EntitySystem
{
public:
	MineLayerModuleControllerSystem(TcpServer* p_server);
	~MineLayerModuleControllerSystem();

	virtual void initialize();
	void processEntities(const vector<Entity*>& p_entities );

private:
	void spawnMine(Transform* p_transform, AglVector3 p_moduleVelocity);
private:
	TcpServer* m_server;
};
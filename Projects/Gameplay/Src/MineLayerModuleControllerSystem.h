#pragma once

#include <EntitySystem.h>
#include "MineLayerModule.h"
#include "Transform.h"
#include "TcpServer.h"

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
class MineLayerModuleControllerSystem: public EntitySystem
{
public:
	MineLayerModuleControllerSystem(TcpServer* p_server);
	~MineLayerModuleControllerSystem();

	virtual void initialize();
	void processEntities(const vector<Entity*>& p_entities );

private:
	void spawnMine(Entity* p_entity);
	void launchMine(Entity* p_entity);
	void updateMine(Entity* p_entity, float p_age);
	void setSpawnAnimation(Entity* p_layer);
private:
	TcpServer* m_server;
};
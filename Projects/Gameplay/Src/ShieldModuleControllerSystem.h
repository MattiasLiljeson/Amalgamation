#pragma once

#include <EntitySystem.h>
#include "ShieldModule.h"
#include "TcpServer.h"
class ServerDynamicPhysicalObjectsSystem;
class PhysicsSystem;
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
	ShieldModuleControllerSystem(TcpServer* p_server);
	~ShieldModuleControllerSystem();
	void initialize() final;
	void processEntities(const vector<Entity*>& p_entities) final;

private:
	void pushEntitiesBack(Entity* p_source, const vector<Entity*>& p_targets);
	bool canTarget(Entity* p_shield, Entity* p_target) const;
	AglVector3 getShieldDir(Entity* p_shield) const;

private:
	TcpServer* m_server;
	ServerDynamicPhysicalObjectsSystem* m_dynamicSystem;
	PhysicsSystem* m_physSystem;

};
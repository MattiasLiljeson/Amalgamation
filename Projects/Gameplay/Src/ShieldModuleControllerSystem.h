#pragma once

#include <EntitySystem.h>
#include "ShieldModule.h"
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
class ShieldModuleControllerSystem: public EntitySystem
{
public:
	ShieldModuleControllerSystem(TcpServer* p_server);
	~ShieldModuleControllerSystem();

	virtual void initialize();
	void processEntities(const vector<Entity*>& p_entities );
private:
	void handleShieldEntity(ShieldModule* p_module, Entity* p_parentEntity, bool p_active);
private:
	TcpServer* m_server;
};
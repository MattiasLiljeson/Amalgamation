#pragma once

#include <EntitySystem.h>
#include "ShieldModule.h"
//#include "OnActivateShieldModule.h"
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

	void initialize();
	void inserted( Entity* p_entity );
	void removed( Entity* p_entity );
	void processEntities(const vector<Entity*>& p_entities) final;
private:
	void processDynamicEntities(const vector<Entity*>& p_dynamics);
	//void handleShieldEntity(ShieldModule* p_module, Entity* p_parentEntity, bool p_active, Entity* p_e);
	void activateShield(Entity* p_shield);
	void deactivateShield(Entity* p_shield);
private:
	TcpServer* m_server;
};
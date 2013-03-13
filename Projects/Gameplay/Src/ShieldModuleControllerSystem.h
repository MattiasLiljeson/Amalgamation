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

	void initialize();
	void inserted( Entity* p_entity );
	void removed( Entity* p_entity );
	void processEntities(const vector<Entity*>& p_entities) final;
private:
	void pushEntitiesBack(Entity* p_source, const vector<Entity*>& p_targets);
	bool canTarget(Entity* p_shield, Entity* p_target) const;
	AglVector3 getShieldDir(Entity* p_shield) const;
	//void handleShieldEntity(ShieldModule* p_module, Entity* p_parentEntity, bool p_active, Entity* p_e);
	void activateShield(Entity* p_shield);
	void deactivateShield(Entity* p_shield);
private:
	TcpServer* m_server;
	ServerDynamicPhysicalObjectsSystem* m_dynamicSystem;
	PhysicsSystem* m_physSystem;

};
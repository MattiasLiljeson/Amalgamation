#pragma once

#include <EntitySystem.h>
#include <PhysicsController.h>
#include "PhysicsBody.h"

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
class PhysicsSystem: public EntitySystem
{
public:
	PhysicsSystem();
	~PhysicsSystem();

	virtual void initialize();
	void processEntities(const vector<Entity*>& p_entities );

	void applyImpulse(int p_bodyID, AglVector3 p_impulse, AglVector3 p_angularImpulse);

private:
	void initializeEntity(Entity* p_entity);

private:
	PhysicsController* m_physicsController;

};
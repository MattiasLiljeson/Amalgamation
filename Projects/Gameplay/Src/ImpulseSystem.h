#pragma once

#include <EntitySystem.h>
#include <PhysicsController.h>
#include "PhysicsBody.h"
#include "Impulse.h"

// =======================================================================================
//                                      ImpulseSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	System for handling communication with the physics library
///        
/// # PhysicsSystem
/// Detailed description.....
/// Created on: 11-12-2012 
///---------------------------------------------------------------------------------------
class ImpulseSystem: public EntitySystem
{
public:
	ImpulseSystem();
	~ImpulseSystem();

	virtual void initialize();
	void processEntities(const vector<Entity*>& p_entities );
private:
	void initializeEntity(Entity* p_entity);

private:
	PhysicsController* m_physicsController;

};
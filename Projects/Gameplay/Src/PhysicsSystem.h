#pragma once

#include <EntitySystem.h>
#include "AglVector3.h"

class PhysicsController;
class PhysicsBody;
class Body;

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
	void handleCompoundBodyDependencies(PhysicsBody* p_bodyComponent, Body* p_rigidBody);

	//Temp
	void addModulesToShip(PhysicsBody* p_body, AglVector3 p_position);
	void queryShipCollision(Entity* ship, const vector<Entity*>& p_others);
private:
	PhysicsController* m_physicsController;

};
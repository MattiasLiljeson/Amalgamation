#pragma once

#include <EntitySystem.h>

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

private:

};
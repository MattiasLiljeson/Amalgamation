#pragma once

#include <EntitySystem.h>

// =======================================================================================
//	DebugMovementSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Moves an entity in a circle.
///        
/// # DebugMovementSystem
/// Components in use: Transform, CircularMovement.
/// Created on: 23-1-2013 
///---------------------------------------------------------------------------------------

class DebugMovementSystem: public EntitySystem
{
public:
	DebugMovementSystem();
	~DebugMovementSystem();
	void processEntities( const vector<Entity*>& p_entities );
};
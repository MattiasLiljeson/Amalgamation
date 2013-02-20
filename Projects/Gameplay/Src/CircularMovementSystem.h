#pragma once
#include <EntitySystem.h>
// =======================================================================================
// CircularMovementSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief...
///        
/// # CircularMovementSystem
/// Detailed description...
/// Created on: 11-2-2013 
///---------------------------------------------------------------------------------------
class CircularMovementSystem: public EntitySystem
{
public:
	CircularMovementSystem();
	~CircularMovementSystem();
	virtual void processEntities(const vector<Entity*>& p_entities) final;

};
#pragma once
#include <EntitySystem.h>
// =======================================================================================
// SineMovementSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief...
///        
/// # SineMovementSystem
/// Detailed description...
/// Created on: 20-3-2013 
///---------------------------------------------------------------------------------------
class SineMovementSystem: public EntitySystem
{
public:
	SineMovementSystem();
	void inserted( Entity* p_entity );
	void processEntities( const vector<Entity*>& p_entities );

};
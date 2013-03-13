#pragma once

#include <EntitySystem.h>

// =======================================================================================
//                               TransformParentHandlerSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # TransformParentHandlerSystem
/// Detailed description.....
/// Created on: 21-1-2013 
///---------------------------------------------------------------------------------------

class EntityParentHandlerSystem : public EntitySystem
{
public:
	EntityParentHandlerSystem();
	virtual void processEntities( const vector<Entity*>& p_entities ) final;

};
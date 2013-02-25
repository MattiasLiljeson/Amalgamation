#pragma once
#include <EntitySystem.h>
// =======================================================================================
// GravityMineEffectSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief...
///        
/// # GravityMineEffectSystem
/// Detailed description...
/// Created on: 25-2-2013 
///---------------------------------------------------------------------------------------
class GravityMineEffectSystem: public EntitySystem
{
public:
	GravityMineEffectSystem();
	virtual void processEntities( const vector<Entity*>& p_entities ) final;

};
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
class AnomalyBombEffectSystem: public EntitySystem
{
public:
	AnomalyBombEffectSystem();
	virtual void processEntities( const vector<Entity*>& p_entities ) final;

};
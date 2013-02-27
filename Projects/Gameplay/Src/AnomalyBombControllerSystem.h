#pragma once
#include <EntitySystem.h>
// =======================================================================================
// AnomalyBombControllerSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief...
///        
/// # AnomalyBombControllerSystem
/// Detailed description...
/// Created on: 27-2-2013 
///---------------------------------------------------------------------------------------
class AnomalyBombControllerSystem: public EntitySystem
{
public:
	AnomalyBombControllerSystem();
	virtual void processEntities( const vector<Entity*>& p_entities ) final;

};
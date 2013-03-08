#pragma once
#include <EntitySystem.h>
// =======================================================================================
// TeslaLightningSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Controls the update logic for 
///        
/// # TeslaLightningSystem
/// Detailed description...
/// Created on: 7-3-2013 
///---------------------------------------------------------------------------------------
class TeslaLightningSystem: public EntitySystem
{
public:
	TeslaLightningSystem();
	void processEntities( const vector<Entity*>& p_entities ) final;

};
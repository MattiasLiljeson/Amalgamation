#pragma once
#include <EntitySystem.h>
// =======================================================================================
// AnomalyAcceleratorModuleControllerSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Controls the AnomalyAcceleratorModule.
///        
/// # AnomalyAcceleratorModuleControllerSystem
/// Detailed description...
/// Created on: 26-2-2013 
///---------------------------------------------------------------------------------------
class AnomalyAcceleratorModuleControllerSystem: public EntitySystem
{
public:
	AnomalyAcceleratorModuleControllerSystem();
	virtual void processEntities( const vector<Entity*>& p_entities ) final;

};
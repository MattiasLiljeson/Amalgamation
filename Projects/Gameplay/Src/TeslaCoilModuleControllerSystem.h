#pragma once
#include <EntitySystem.h>
class Entity;
class TeslaCoilModule;
class Transform;
class PhysicsBody;
class NetworkSynced;
// =======================================================================================
// TeslaCoilModuleControllerSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief...
///        
/// # TeslaCoilModuleControllerSystem
/// Detailed description...
/// Created on: 1-3-2013 
///---------------------------------------------------------------------------------------
class TeslaCoilModuleControllerSystem: public EntitySystem
{
public:
	TeslaCoilModuleControllerSystem();
	void processEntities( const vector<Entity*>& p_entities ) final;

private:
	void fireTeslaCoil(Entity* p_teslaEntity, TeslaCoilModule* p_teslaModule,
		Transform* p_teslaTransform, NetworkSynced* p_teslaNetsync);
	float calculateMultiplier( float p_distance, float p_optimalRange, float p_range );
};
#pragma once
#include <EntitySystem.h>
class Entity;
class TeslaCoilModule;
class Transform;
class PhysicsBody;
class NetworkSynced;
class ShipModule;
class TcpServer;
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
	TeslaCoilModuleControllerSystem(TcpServer* p_server);
	void processEntities( const vector<Entity*>& p_entities ) final;

private:
	void fireTeslaCoil(Entity* p_teslaEntity, TeslaCoilModule* p_teslaModule,
		Transform* p_teslaTransform, NetworkSynced* p_teslaNetsync,
		ShipModule* p_teslaShipModule);
	float calculateHitChance( float p_distance, float p_optimalRange, float p_range );
	bool canTarget(ShipModule* p_teslaModule, ShipModule* p_otherModule) const;

	void playFireAnimation(Entity* p_tesla);
	void playDeathAnimation(Entity* p_tesla);

private:
	TcpServer* m_server;

};
#pragma once
#include <EntitySystem.h>
#include <AglVector3.h>
class Transform;
class ShipModule;
class TcpServer;
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
	AnomalyAcceleratorModuleControllerSystem(TcpServer* p_server);
	virtual void processEntities( const vector<Entity*>& p_entities ) final;
	void spawnAnomalyBomb( Transform* p_transform, AglVector3 p_moduleVelocity, 
		ShipModule* p_module );

private:
	TcpServer* m_server;

};
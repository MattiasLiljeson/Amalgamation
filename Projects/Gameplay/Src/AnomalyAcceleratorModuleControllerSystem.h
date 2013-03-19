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
	void spawnAnomalyBomb(Entity* p_entity);
	void updateBomb(Entity* p_entity, float p_age);

private:
	void playReleaseAnimation(Entity* p_accelerator);
	void playOpenAnimation(Entity* p_accelerator);
	void launchBomb(Entity* p_entity);

private:
	TcpServer* m_server;

};
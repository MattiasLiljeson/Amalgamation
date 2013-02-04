#pragma once
#include <EntitySystem.h>
class TcpServer;
// =======================================================================================
// SpeedBoostModuleControllerSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief...
///        
/// # SpeedBoostModuleControllerSystem
/// Detailed description...
/// Created on: 4-2-2013 
///---------------------------------------------------------------------------------------
class SpeedBoostModuleControllerSystem: public EntitySystem
{
public:
	SpeedBoostModuleControllerSystem(TcpServer* p_server);
	~SpeedBoostModuleControllerSystem();
	void inserted( Entity* p_entity );

private:
	TcpServer* m_server;
};
#pragma once
#include <EntitySystem.h>
class TcpServer;
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
	AnomalyBombControllerSystem(TcpServer* p_server);
	virtual void processEntities( const vector<Entity*>& p_entities ) final;

private:
	TcpServer* m_server;

};
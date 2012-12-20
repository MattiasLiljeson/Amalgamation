#pragma once

// ES
#include <EntitySystem.h>

class TcpServer;
class PhysicsSystem;

// =======================================================================================
//                                      NetworkInputHandlerSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # NetworkInputHandlerSystem
/// Detailed description.....
/// Created on: 20-12-2012 
///---------------------------------------------------------------------------------------

class NetworkInputHandlerSystem : public EntitySystem
{
public:
	NetworkInputHandlerSystem( TcpServer* p_server );
	~NetworkInputHandlerSystem();

	void initialize();

	void processEntities( const vector<Entity*>& p_entities );

private:
	TcpServer* m_server;
	PhysicsSystem* m_physics;
};
#pragma once

// ES
#include <EntitySystem.h>

class TcpServer;
class PhysicsSystem;

// =======================================================================================
//                                      ServerPacketHandlerSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # NetworkInputHandlerSystem
/// Detailed description.....
/// Created on: 20-12-2012 
///---------------------------------------------------------------------------------------

class ServerPacketHandlerSystem : public EntitySystem
{
public:
	ServerPacketHandlerSystem( TcpServer* p_server );
	~ServerPacketHandlerSystem();

	void initialize();

	void processEntities( const vector<Entity*>& p_entities );

private:
	TcpServer* m_server;
	PhysicsSystem* m_physics;
};
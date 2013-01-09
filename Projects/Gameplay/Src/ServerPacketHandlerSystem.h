#pragma once

#include <map>
#include <EntitySystem.h>
#include "ClientInfo.h"

class TcpServer;
class PhysicsSystem;

using namespace std;

// =======================================================================================
//                                  ServerPacketHandlerSystem
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
	TcpServer*		m_server;
	PhysicsSystem*	m_physics;
	map<int,ClientInfo> m_clients; ///< The key is the clientID
	
	float m_timer;
	float m_timerStartValue;
};
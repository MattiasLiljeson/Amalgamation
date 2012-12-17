#pragma once

// ES
#include <EntitySystem.h>

// Components
#include "Transform.h"
#include "NetworkSynced.h"
#include "PhysicsBody.h"

// NetComm
#include <TcpServer.h>

#include "PacketType.h"
#include "NetworkType.h"
#include "PhysicsSystem.h"

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
#pragma once

// ES
#include <EntitySystem.h>

// NetComm
#include <TcpServer.h>

#include "Transform.h"

#include "PacketType.h"
#include "PlayerInputAction.h"

class NetworkInputHandlerSystem : public EntitySystem
{
public:
	NetworkInputHandlerSystem( TcpServer* p_server );
	~NetworkInputHandlerSystem();

	void initialize();

	void processEntities( const vector<Entity*>& p_entities );

private:
	TcpServer* m_server;
};
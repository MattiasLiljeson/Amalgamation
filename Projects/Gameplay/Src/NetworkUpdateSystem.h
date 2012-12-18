#pragma once

#include <EntitySystem.h>
#include <TcpServer.h>
#include <Entity.h>

#include "PacketType.h"
#include "NetworkSynced.h"
#include "Transform.h"

class NetworkUpdateSystem: public EntitySystem
{
public:
	NetworkUpdateSystem( TcpServer* p_server );

	~NetworkUpdateSystem();
	
	void processEntities( const vector<Entity*>& p_entities );

	void initialize();

private:
	TcpServer* m_server;

};
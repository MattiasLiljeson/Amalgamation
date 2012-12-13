#pragma once

#include <Entity.h>
#include <EntitySystem.h>
#include <Component.h>
#include <ComponentType.h>
#include <SystemType.h>

#include <Packet.h>
#include <TcpClient.h>

#include <AglVector3.h>

#include "GraphicsBackendSystem.h"
#include "NetworkSynced.h"
#include "NetworkType.h"
#include "PacketType.h"
#include "Transform.h"
#include "RenderInfo.h"

class NetworkCommunicatorSystem: public EntitySystem
{
public:
	NetworkCommunicatorSystem( TcpClient* p_tcpClient );

	~NetworkCommunicatorSystem();

	virtual void processEntities( const vector<Entity*>& p_entities );

	virtual void initialize();

private:
	TcpClient* m_tcpClient;

};
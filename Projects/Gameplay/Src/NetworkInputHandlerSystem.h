#pragma once

// ES
#include <EntitySystem.h>

// NetComm
#include <TcpServer.h>

class NetworkInputHandlerSystem : public EntitySystem
{
public:
	NetworkInputHandlerSystem( TcpServer* p_server );
	~NetworkInputHandlerSystem();

	virtual void process();
	virtual void initialize();

private:
	TcpServer* m_server;
};
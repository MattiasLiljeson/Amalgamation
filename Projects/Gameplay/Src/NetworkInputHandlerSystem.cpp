#include "NetworkInputHandlerSystem.h"

NetworkInputHandlerSystem::NetworkInputHandlerSystem( TcpServer* p_server )
	: EntitySystem( SystemType::NetworkInputHandlerSystem/*, 1, ComponentType::NetworkSynced*/)
{
	m_server = p_server;
}

NetworkInputHandlerSystem::~NetworkInputHandlerSystem()
{

}

void NetworkInputHandlerSystem::process()
{

}

void NetworkInputHandlerSystem::initialize()
{

}

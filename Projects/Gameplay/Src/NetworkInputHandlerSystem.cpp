#include "NetworkInputHandlerSystem.h"

NetworkInputHandlerSystem::NetworkInputHandlerSystem( TcpServer* p_server )
	: EntitySystem( SystemType::NetworkInputHandlerSystem, 1,
	ComponentType::NetworkSynced )
{
	m_server = p_server;
}

NetworkInputHandlerSystem::~NetworkInputHandlerSystem()
{

}


void NetworkInputHandlerSystem::initialize()
{

}

void NetworkInputHandlerSystem::processEntities( const vector<Entity*>& p_entities )
{
	while( m_server->hasNewPackets() )
	{
		Packet packet = m_server->popNewPacket();

		char packetType;
		packet >> packetType;

		if(packetType == (char)PacketType::PlayerInput)
		{
			char playerInputAction;
			packet >> playerInputAction;

			if(playerInputAction == (char)PlayerInputAction::ThrustForward)
			{
				bool thrustState;
				packet >> thrustState;

				for( unsigned int i=0; i<p_entities.size(); i++ )
				{
					// TODO: Need to know who sent the packet!
				}
			}
		}
	}
}

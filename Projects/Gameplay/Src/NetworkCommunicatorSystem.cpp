#include "NetworkCommunicatorSystem.h"

NetworkCommunicatorSystem::NetworkCommunicatorSystem( TcpClient* p_tcpClient )
	: EntitySystem( SystemType::NetworkCommunicatorSystem, 1, 
					ComponentType::NetworkSynced)
{
	m_tcpClient = p_tcpClient;
}

NetworkCommunicatorSystem::~NetworkCommunicatorSystem()
{

}

void NetworkCommunicatorSystem::processEntities( const vector<Entity*>& p_entities )
{

	while (m_tcpClient->hasNewPackets())
	{
		Packet packet = m_tcpClient->popNewPacket();

		char packetType;
		
		packet >> packetType;
		if (packetType == (char)PacketType::EntityCreation)
		{
			char networkType;
			
			packet >> networkType;
			if (networkType == (char)NetworkType::Ship)
			{
				int			owner;
				int			networkId;
				AglVector3	position;

				packet >> owner >> networkId >> position.x >> position.y >> position.z;

				Entity* e = NULL;
				e = m_world->createEntity();
				
				e->addComponent(ComponentType::Transform,
					new Transform(position.x, position.y, position.z));
				e->addComponent(ComponentType::NetworkSynced,
					new NetworkSynced(networkId, owner, NetworkType::Ship));
				e->addComponent(ComponentType::RenderInfo,
					new RenderInfo());

				m_world->addEntity(e);
			}
		}
	}
}

void NetworkCommunicatorSystem::initialize()
{

}

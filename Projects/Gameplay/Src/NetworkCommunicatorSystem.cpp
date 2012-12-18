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

				packet >> owner >> networkId >> position;

				int meshId = static_cast<GraphicsBackendSystem*>(m_world->getSystem(
					SystemType::GraphicsBackendSystem ))->getMeshId("P_cube");

				Entity* e = NULL;
				e = m_world->createEntity();
				
				e->addComponent(ComponentType::Transform,
					new Transform(position.x, position.y, position.z));
				e->addComponent(ComponentType::NetworkSynced,
					new NetworkSynced(networkId, owner, NetworkType::Ship));
				e->addComponent(ComponentType::RenderInfo,
					new RenderInfo( meshId ));

				m_world->addEntity(e);
			}
		}
		else if (packetType == (char)PacketType::EntityUpdate)
		{
			char networkType;
			
			packet >> networkType;
			if (networkType == (char)NetworkType::Ship)
			{
				int			networkId;
				AglVector3	position;

				packet >> networkId >> position;



				// HACK: This is VERY inefficient for large amount of
				// network-synchronized entities. (Solve later)
				for( unsigned int i=0; i<p_entities.size(); i++ )
				{
					NetworkSynced* netSync = NULL;
					netSync = static_cast<NetworkSynced*>(
						m_world->getComponentManager()->getComponent(
						p_entities[i]->getIndex(), ComponentType::NetworkSynced ) );
					if( netSync->getNetworkIdentity() == networkId )
					{
						Transform* transform = NULL;
						transform = static_cast<Transform*>(
							m_world->getComponentManager()->getComponent(
							p_entities[i]->getIndex(), ComponentType::Transform ) );
						transform->setTranslation( position );
						if (transform->getTranslation().z == 0.0f)
						{
							int a = 0;
						}
					}
				}
			}
		}
		else if(packetType == (char)PacketType::InitCredentials)
		{
			char networkType;

			packet >> networkType;
			if(networkType == (char)NetworkType::Identity)
			{
				int id;
				packet >> id;

				m_tcpClient->setId( id );
			}
		}
	}
}

void NetworkCommunicatorSystem::initialize()
{

}

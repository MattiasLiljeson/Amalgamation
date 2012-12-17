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
				int				owner;
				int				networkId;
				AglVector3		position;
				AglQuaternion	rotation;
				AglVector3		scale;

				packet >> owner >> networkId >> position >> rotation >> scale;

				int meshId = static_cast<GraphicsBackendSystem*>(m_world->getSystem(
					SystemType::GraphicsBackendSystem ))->getMeshId("P_cube");


				Entity* e = NULL;
				e = m_world->createEntity();
				
				
				int shipId = e->getIndex();

				if(m_tcpClient->getId() == owner)
				{
					e->addComponent( ComponentType::ShipController,
						new ShipController(0.3f,3.0f) );
				}
				e->addComponent(ComponentType::Transform,
					new Transform(position, rotation, scale));
				e->addComponent(ComponentType::NetworkSynced,
					new NetworkSynced(networkId, owner, NetworkType::Ship));
				e->addComponent(ComponentType::RenderInfo,
					new RenderInfo( meshId ));

				m_world->addEntity(e);


				if(owner == m_tcpClient->getId())
				{
					// A camera from which the world is rendered.
					Entity* entity = NULL;
					Component* component = NULL;
					entity = m_world->createEntity();
					component = new CameraInfo( 800/(float)600 );
					entity->addComponent( ComponentType::CameraInfo, component );
					component = new Input();
					entity->addComponent( ComponentType::Input, component );
					component = new Transform( -5.0f, 0.0f, -5.0f );
					entity->addComponent( ComponentType::Transform, component );
					component = new LookAtEntity(shipId, AglVector3(0,3,-10));
					entity->addComponent( ComponentType::LookAtEntity, component );
					m_world->addEntity(entity);
				}
			}
		}
		else if (packetType == (char)PacketType::EntityUpdate)
		{
			char networkType;
			
			packet >> networkType;
			if (networkType == (char)NetworkType::Ship)
			{
				int			networkId;
				AglVector3		position;
				AglQuaternion	rotation;
				AglVector3		scale;

				packet >> networkId >> position >> rotation >> scale;



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
						transform->setRotation( rotation );
						transform->setScale( scale );
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

#include "NetworkCommunicatorSystem.h"
#include "AudioListener.h"
#include "PhysicsBody.h"
#include "BodyInitData.h"
#include <AntTweakBarWrapper.h>
#include <Entity.h>
#include <Component.h>
#include <ComponentType.h>
#include <SystemType.h>
#include <AntTweakBarWrapper.h>

#include <Packet.h>
#include <TcpClient.h>

// Components
#include "NetworkSynced.h"
#include "Transform.h"
#include "RenderInfo.h"
#include "ShipController.h"
#include "CameraInfo.h"
#include "MainCamera.h"
#include "Input.h"
#include "LookAtEntity.h"
#include "PlayerScore.h"

#include "GraphicsBackendSystem.h"
#include "NetworkType.h"
#include "PacketType.h"



NetworkCommunicatorSystem::NetworkCommunicatorSystem( TcpClient* p_tcpClient )
	: EntitySystem( SystemType::NetworkCommunicatorSystem, 1, 
					ComponentType::NetworkSynced)
{
	m_tcpClient = p_tcpClient;

	m_timer = m_timerStartValue = 0.25f;
	m_currentPing = 0;
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
			NetworkEntityCreationPacket data = readCreationPacket(packet);
			if (data.networkType == (char)NetworkType::Ship )
			{
				int shipMeshId = static_cast<GraphicsBackendSystem*>(m_world->getSystem(
					SystemType::GraphicsBackendSystem ))->getMeshId("Ship.agl");

				Transform* transform = new Transform( data.position, data.rotation, 
					data.scale);

				Entity* entity = NULL;
				Component* component;
				// Create a "spaceship"
				entity = m_world->createEntity();
				component = new RenderInfo( shipMeshId );
				entity->addComponent( ComponentType::RenderInfo, component );
				component = transform;
				entity->addComponent( ComponentType::Transform, component );

				// HACK: Should be located in another entity:
				component = new PlayerScore();
				entity->addComponent( ComponentType::PlayerScore, component );

				if(m_tcpClient->getId() == data.owner)
				{
					// If "this client" is the entity owner, it may control the ship:
					component = new ShipController(5.0f, 50.0f);
					entity->addComponent( ComponentType::ShipController, component );
				}
				entity->addComponent(ComponentType::NetworkSynced,
					new NetworkSynced(data.networkId, data.owner, NetworkType::Ship));
				m_world->addEntity(entity);
				
				
				int shipId = entity->getIndex();

				if(data.owner == m_tcpClient->getId())
				{
					float aspectRatio = 
						static_cast<GraphicsBackendSystem*>(m_world->getSystem(
						SystemType::GraphicsBackendSystem ))->getAspectRatio();

					// A camera from which the world is rendered.
					entity = m_world->createEntity();
					component = new CameraInfo( aspectRatio );
					entity->addComponent( ComponentType::CameraInfo, component );
					component = new MainCamera();
					entity->addComponent( ComponentType::MainCamera, component );
					//component = new Input();
					//entity->addComponent( ComponentType::Input, component );
					component = new Transform( -5.0f, 0.0f, -5.0f );
					entity->addComponent( ComponentType::Transform, component );
					component = new LookAtEntity(shipId, AglVector3(0,3,-10),10.0f,10.0f);
					entity->addComponent( ComponentType::LookAtEntity, component );
					component = new AudioListener();
					entity->addComponent(ComponentType::AudioListener, component);
					m_world->addEntity(entity);

					/************************************************************************/
					/* Debug information only and there is no need for this to run the code */
					/************************************************************************/
					AntTweakBarWrapper::getInstance()->addWriteVariable("Master_volume",
						TwType::TW_TYPE_FLOAT, 
						static_cast<AudioListener*>(component)->getMasterVolumeRef(),
						"group=Sound min=0 max=10 step=0.001 precision=3");

					AntTweakBarWrapper::getInstance()->addReadOnlyVariable( "NetId",
						TwType::TW_TYPE_INT32,
						m_tcpClient->getIdPointer(), "" );
				}
			}
			else if ( data.networkType == (char)NetworkType::Prop )
			{
				Entity* entity;
				Component* component;
				//b1
				entity = m_world->createEntity();
				//component = new RenderInfo( cubeMeshId );
				//entity->addComponent( ComponentType::RenderInfo, component );
				component = new Transform(data.position, data.rotation, data.scale);
				entity->addComponent( ComponentType::Transform, component );

				// The b1 entity should be synced over the network!
				component = new NetworkSynced(data.networkId, -1, NetworkType::Prop);
				entity->addComponent(ComponentType::NetworkSynced, component);

				int meshId = static_cast<GraphicsBackendSystem*>(m_world->getSystem(
					SystemType::GraphicsBackendSystem ))->getMeshId("P_cube");
				component = new RenderInfo(meshId);
				entity->addComponent(ComponentType::RenderInfo, component);

				m_world->addEntity(entity);
			}


		}
		else if (packetType == (char)PacketType::EntityUpdate)
		{
			NetworkEntityUpdatePacket data = readUpdatePacket(packet);
			if (data.networkType == (char)NetworkType::Ship ||
				data.networkType == (char)NetworkType::Prop)
			{

				if(data.networkType == (char)NetworkType::Prop)
				{
					data.networkType = data.networkType;
				}
				// HACK: This is VERY inefficient for large amount of
				// network-synchronized entities. (Solve later)
				for( unsigned int i=0; i<p_entities.size(); i++ )
				{
					NetworkSynced* netSync = NULL;
					netSync = static_cast<NetworkSynced*>(
						m_world->getComponentManager()->getComponent(
						p_entities[i]->getIndex(), ComponentType::NetworkSynced ) );
					if( netSync->getNetworkIdentity() == data.networkId )
					{
						Transform* transform = NULL;
						transform = static_cast<Transform*>(
							m_world->getComponentManager()->getComponent(
							p_entities[i]->getIndex(), ComponentType::Transform ) );
						transform->setTranslation( data.position );
						transform->setRotation( data.rotation );
						transform->setScale( data.scale );
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
		else if(packetType == (char)PacketType::ScoresUpdate)
		{
			NetworkScoreUpdatePacket scoreUpdateData;
			scoreUpdateData = readScorePacket( packet );

			// HACK: This is VERY inefficient for large amount of
			// network-synchronized entities. (Solve later)
			for( unsigned int i=0; i<p_entities.size(); i++ )
			{
				NetworkSynced* netSync = NULL;
				netSync = static_cast<NetworkSynced*>(
					m_world->getComponentManager()->getComponent(
					p_entities[i]->getIndex(), ComponentType::NetworkSynced ) );

				if( netSync->getNetworkIdentity() == scoreUpdateData.networkId )
				{
					PlayerScore* scoreComponent = static_cast<PlayerScore*>(
						m_world->getComponentManager()->getComponent(
						p_entities[i]->getIndex(), ComponentType::PlayerScore ) );
					if( scoreComponent )
					{
						scoreComponent->setScore( scoreUpdateData.score );
					}
				}
			}
		}
		else if(packetType == (char)PacketType::Ping)
		{
			// TODO: Implement later.
		}
		else if(packetType == (char)PacketType::Pong)
		{
			SYSTEMTIME serverTimestamp;
			packet >> serverTimestamp;
			m_currentPing = serverTimestamp.wSecond * 1000 - m_timestamp.wSecond * 1000 +
				serverTimestamp.wMilliseconds - m_timestamp.wMilliseconds;
		}
	}

	m_timer -= m_world->getDelta();
	if( m_timer <= 0 )
	{
		m_timer = m_timerStartValue;

		GetSystemTime( &m_timestamp );

		Packet packet;
		packet << (char)NetworkType::Other;
		packet << (char)PacketType::Ping;

		m_tcpClient->sendPacket( packet );
	}
}

void NetworkCommunicatorSystem::initialize()
{
	AntTweakBarWrapper::getInstance()->addReadOnlyVariable( "Ping", TwType::TW_TYPE_INT32,
		&m_currentPing, "" );
}

NetworkEntityCreationPacket NetworkCommunicatorSystem::readCreationPacket( 
	Packet& p_packet )
{
	NetworkEntityCreationPacket data;
	p_packet >> data.networkType 
		>> data.owner 
		>> data.networkId 
		>> data.position 
		>> data.rotation 
		>> data.scale;
	return data;
}

NetworkEntityUpdatePacket NetworkCommunicatorSystem::readUpdatePacket( Packet& p_packet )
{
	NetworkEntityUpdatePacket data;
	p_packet >> data.networkType
		>> data.networkId 
		>> data.position 
		>> data.rotation 
		>> data.scale;
	return data;
}

NetworkScoreUpdatePacket NetworkCommunicatorSystem::readScorePacket( Packet& p_packet )
{
	NetworkScoreUpdatePacket data;
	p_packet >> data.networkId;
	p_packet >> data.score;

	return data;
}

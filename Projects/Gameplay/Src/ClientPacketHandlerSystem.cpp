#include "ClientPacketHandlerSystem.h"
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
#include "ShipFlyController.h"
#include "CameraInfo.h"
#include "MainCamera.h"
#include "Input.h"
#include "LookAtEntity.h"
#include "PlayerScore.h"
#include "GameplayTags.h"

#include "GraphicsBackendSystem.h"
#include "EntityType.h"
#include "PacketType.h"



ClientPacketHandlerSystem::ClientPacketHandlerSystem( TcpClient* p_tcpClient )
	: EntitySystem( SystemType::ClientPacketHandlerSystem, 1, 
					ComponentType::NetworkSynced)
{
	m_tcpClient = p_tcpClient;

	m_currentPing = 0;

	m_numberOfSentPackets = 0;
	m_numberOfReceivedPackets = 0;
	m_totalDataSent = 0;
	m_totalDataReceived = 0;
	m_dataSentPerSecond = 0;
	m_dataReceivedPerSecond = 0;
	m_dataSentCounter = 0;
	m_dataReceivedCounter;

	m_timerPerSecond = 1.0f;
}

ClientPacketHandlerSystem::~ClientPacketHandlerSystem()
{

}

void ClientPacketHandlerSystem::processEntities( const vector<Entity*>& p_entities )
{
	updateCounters();

	while (m_tcpClient->hasNewPackets())
	{
		Packet packet = m_tcpClient->popNewPacket();

		char packetType;
		
		packet >> packetType;
#pragma region EntityCreation
		if (packetType == (char)PacketType::EntityCreation)
		{
			handleEntityCreationPacket(packet);
		}
#pragma endregion

#pragma region EntityUpdate
		else if (packetType == (char)PacketType::EntityUpdate)
		{
			NetworkEntityUpdatePacket data = readUpdatePacket(packet);
			if (data.networkType == (char)EntityType::Ship ||
				data.networkType == (char)EntityType::Prop)
			{

				if(data.networkType == (char)EntityType::Prop)
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
#pragma endregion 
		else if(packetType == (char)PacketType::WelcomePacket)
		{
			handleWelcomePacket(packet);
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
			float serverTime;
			packet >> serverTime;

			Packet response((char)PacketType::Pong);
			response << serverTime;

			m_tcpClient->sendPacket(response);
		}
		else if(packetType == (char)PacketType::Pong)
		{
			float totalElapsedTime = m_world->getElapsedTime();
			float timeWhenSent;

			packet >> timeWhenSent;

			/************************************************************************/
			/* Convert from seconds to milliseconds.								*/
			/************************************************************************/
			m_currentPing = (totalElapsedTime - timeWhenSent)*1000.0f;
		}
		else if(packetType == (char)PacketType::UpdateClientStats)
		{
			packet >> m_currentPing;
		}
	}
}

void ClientPacketHandlerSystem::handleWelcomePacket( Packet p_packet )
{
	int id;
	p_packet >> id;
	m_tcpClient->setId( id );

	/************************************************************************/
	/* Debug info!															*/
	/************************************************************************/
	AntTweakBarWrapper::getInstance()->addReadOnlyVariable( 
		AntTweakBarWrapper::NETWORK,
		"NetId", TwType::TW_TYPE_INT32,
		m_tcpClient->getIdPointer(), "" );
}

void ClientPacketHandlerSystem::handleEntityCreationPacket( Packet p_packet )
{
	NetworkEntityCreationPacket data = readCreationPacket(p_packet);

	if (data.networkType == (char)EntityType::Ship )
	{
		int shipMeshId = static_cast<GraphicsBackendSystem*>(m_world->getSystem(
			SystemType::GraphicsBackendSystem ))->getMeshId("Ship.agl");

		Entity* entity = NULL;
		Component* component;

		/************************************************************************/
		/* This ship creation code have to be located somewhere else.			*/
		/************************************************************************/
		entity = m_world->createEntity();
		component = new RenderInfo( shipMeshId );
		entity->addComponent( ComponentType::RenderInfo, component );
		Transform* transform = new Transform( data.position, data.rotation, 
			data.scale);
		component = transform;
		entity->addComponent( ComponentType::Transform, component );

		/************************************************************************/
		/* HACK: Score should probably be located in another entity.			*/
		/************************************************************************/
		//component = new PlayerScore();
		//entity->addComponent( ComponentType::PlayerScore, component );

		/************************************************************************/
		/* Check if the owner is the same as this client.						*/
		/************************************************************************/
		if(m_tcpClient->getId() == data.owner)
		{
			component = new ShipFlyController(5.0f, 50.0f);
			entity->addComponent( ComponentType::ShipFlyController, component );
		}
		entity->addComponent(ComponentType::NetworkSynced,
			new NetworkSynced(data.networkId, data.owner, EntityType::Ship));
		m_world->addEntity(entity);

		if(data.owner == m_tcpClient->getId())
		{
			int shipId = entity->getIndex();
			float aspectRatio = 
				static_cast<GraphicsBackendSystem*>(m_world->getSystem(
				SystemType::GraphicsBackendSystem ))->getAspectRatio();

			// A camera from which the world is rendered.
			entity = m_world->createEntity();
			component = new CameraInfo( aspectRatio );
			entity->addComponent( ComponentType::CameraInfo, component );
			component = new MainCamera();
			entity->addComponent( ComponentType::MainCamera, component );
			component = new Transform( -5.0f, 0.0f, -5.0f );
			entity->addComponent( ComponentType::Transform, component );
			component = new LookAtEntity(shipId, AglVector3(0,3,-10),
				AglQuaternion::identity(),0.0f,10.0f);
			// default tag is follow
			entity->addTag(ComponentType::TAG_LookAtFollowMode, new LookAtFollowMode_TAG());
			component = new AudioListener();
			entity->addComponent(ComponentType::AudioListener, component);
			m_world->addEntity(entity);

			/************************************************************************/
			/* This is where the audio listener is created and therefor the master  */
			/* volume is added to Ant Tweak Bar here.								*/
			/************************************************************************/
			AntTweakBarWrapper::getInstance()->addWriteVariable( 
				AntTweakBarWrapper::OVERALL,
				"Master_volume", TwType::TW_TYPE_FLOAT, 
				static_cast<AudioListener*>(component)->getMasterVolumeRef(),
				"group=Sound min=0 max=10 step=0.001 precision=3");
		}
	}
	else if ( data.networkType == (char)EntityType::Prop )
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
		component = new NetworkSynced(data.networkId, -1, EntityType::Prop);
		entity->addComponent(ComponentType::NetworkSynced, component);

		int meshId = static_cast<GraphicsBackendSystem*>(m_world->getSystem(
			SystemType::GraphicsBackendSystem ))->getMeshId("P_cube");
		component = new RenderInfo(meshId);
		entity->addComponent(ComponentType::RenderInfo, component);

		m_world->addEntity(entity);
	}
}

void ClientPacketHandlerSystem::initialize()
{
	AntTweakBarWrapper::getInstance()->modifyTheRefreshRate(AntTweakBarWrapper::NETWORK,
		0.1f );

	AntTweakBarWrapper::getInstance()->addReadOnlyVariable(
		AntTweakBarWrapper::getInstance()->BarType::OVERALL,
		"Ping", TwType::TW_TYPE_FLOAT,
		&m_currentPing, "" );

	/************************************************************************/
	/* Per frame data.														*/
	/************************************************************************/
	AntTweakBarWrapper::getInstance()->addReadOnlyVariable(
		AntTweakBarWrapper::getInstance()->BarType::NETWORK,
		"Received packets", TwType::TW_TYPE_UINT32,
		&m_numberOfReceivedPackets, "group='Per frame'" );

	AntTweakBarWrapper::getInstance()->addReadOnlyVariable(
		AntTweakBarWrapper::getInstance()->BarType::NETWORK,
		"Sent packets", TwType::TW_TYPE_UINT32,
		&m_numberOfSentPackets, "group='Per frame'" );

	AntTweakBarWrapper::getInstance()->addReadOnlyVariable(
		AntTweakBarWrapper::getInstance()->BarType::NETWORK,
		"Data received/f", TwType::TW_TYPE_UINT32,
		&m_totalDataReceived, "group='Per frame'" );

	AntTweakBarWrapper::getInstance()->addReadOnlyVariable(
		AntTweakBarWrapper::getInstance()->BarType::NETWORK,
		"Data sent/f", TwType::TW_TYPE_UINT32,
		&m_totalDataSent, "group='Per frame'" );

	/************************************************************************/
	/* Per second data.														*/
	/************************************************************************/
	AntTweakBarWrapper::getInstance()->addReadOnlyVariable(
		AntTweakBarWrapper::getInstance()->BarType::NETWORK,
		"Data received/s", TwType::TW_TYPE_UINT32,
		&m_dataReceivedPerSecond, "group='Per second'" );

	AntTweakBarWrapper::getInstance()->addReadOnlyVariable(
		AntTweakBarWrapper::getInstance()->BarType::NETWORK,
		"Data sent/s", TwType::TW_TYPE_UINT32,
		&m_dataSentPerSecond, "group='Per second'" );

}

NetworkEntityCreationPacket ClientPacketHandlerSystem::readCreationPacket( 
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

NetworkEntityUpdatePacket ClientPacketHandlerSystem::readUpdatePacket( Packet& p_packet )
{
	NetworkEntityUpdatePacket data;
	p_packet >> data.networkType
		>> data.networkId 
		>> data.position 
		>> data.rotation 
		>> data.scale;
	return data;
}

NetworkScoreUpdatePacket ClientPacketHandlerSystem::readScorePacket( Packet& p_packet )
{
	NetworkScoreUpdatePacket data;
	p_packet >> data.networkId;
	p_packet >> data.score;

	return data;
}

void ClientPacketHandlerSystem::updateCounters()
{
	m_numberOfReceivedPackets = m_tcpClient->getNumberOfReceivedPackets();
	m_tcpClient->resetNumberOfReceivedPackets();

	m_numberOfSentPackets = m_tcpClient->getNumberOfSentPackets();
	m_tcpClient->resetNumberOfSentPackets();

	m_totalDataReceived = m_tcpClient->getTotalDataReceived();
	m_tcpClient->resetTotalDataReceived();
	m_dataReceivedCounter += m_totalDataReceived;

	m_totalDataSent = m_tcpClient->getTotalDataSent();
	m_tcpClient->resetTotalDataSent();
	m_dataSentCounter += m_totalDataSent;

	m_timerPerSecond -= m_world->getDelta();
	if( m_timerPerSecond <= 0 )
	{
		m_timerPerSecond = 1.0f;

		m_dataSentPerSecond = m_dataSentCounter;
		m_dataReceivedPerSecond = m_dataReceivedCounter;

		m_dataSentCounter = 0;
		m_dataReceivedCounter = 0;
	}
}
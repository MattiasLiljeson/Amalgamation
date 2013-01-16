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
#include "PlayerCameraController.h"

#include "GraphicsBackendSystem.h"
#include "EntityType.h"
#include "PacketType.h"
#include "PickComponent.h"

// Debug
#include <DebugUtil.h>
#include "ShipEditController.h"
#include "ConnectionPointSet.h"
#include "TimerSystem.h"
#include "PingPacket.h"
#include "PongPacket.h"
#include "EntityUpdatePacket.h"
#include "EntityCreationPacket.h"
#include "WelcomePacket.h"
#include "UpdateClientStatsPacket.h"

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
	m_dataReceivedCounter = 0;
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
		
		packetType = packet.getPacketType();

#pragma region EntityUpdate
		if (packetType == (char)PacketType::EntityUpdate)
		{
			EntityUpdatePacket data;
			data.unpack(packet);
			if (data.entityType == (char)EntityType::Ship ||
				data.entityType == (char)EntityType::Prop ||
				data.entityType == (char)EntityType::ShipModule)
			{

				// HACK: This is VERY inefficient for large amount of
				// network-synchronized entities. (Solve later)
				for( unsigned int i=0; i<p_entities.size(); i++ )
				{
					NetworkSynced* netSync = NULL;
					netSync = static_cast<NetworkSynced*>(
						m_world->getComponentManager()->getComponent(
						p_entities[i]->getIndex(), ComponentType::NetworkSynced ) );
					if( netSync->getNetworkIdentity() == data.networkIdentity )
					{
						Transform* transform = NULL;
						transform = static_cast<Transform*>(
							m_world->getComponentManager()->getComponent(
							p_entities[i]->getIndex(), ComponentType::Transform ) );
						transform->setTranslation( data.translation );
						transform->setRotation( data.rotation );
						transform->setScale( data.scale );
					}
				}
			}
		}
#pragma endregion 
		/************************************************************************/
		/* Score is now included in player update client stats packets.			*/
		/************************************************************************/
		/*
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
		*/
		else if(packetType == (char)PacketType::Ping)
		{
			PingPacket pingPacket;
			pingPacket.unpack( packet );

			PongPacket pongPacket;
			pongPacket.timeStamp = pingPacket.timeStamp;
			m_tcpClient->sendPacket( pongPacket.pack() );
		}
		else if(packetType == (char)PacketType::Pong)
		{
			PongPacket pongPacket;
			pongPacket.unpack(packet);
			float totalElapsedTime = m_world->getElapsedTime();

			/************************************************************************/
			/* Convert from seconds to milliseconds.								*/
			/************************************************************************/
			m_currentPing = (totalElapsedTime - pongPacket.timeStamp)*1000.0f;
		}
		else if(packetType == (char)PacketType::UpdateClientStats)
		{
			UpdateClientStatsPacket updateClientPacket;
			updateClientPacket.unpack(packet);
			m_currentPing = updateClientPacket.ping;
		}
		else if(packetType == (char)PacketType::EntityCreation)
		{
			EntityCreationPacket data;
			data.unpack(packet);
			handleEntityCreationPacket(data);
		}
		else if(packetType == (char)PacketType::WelcomePacket)
		{
			handleWelcomePacket(packet);
		}
	}
}

void ClientPacketHandlerSystem::handleWelcomePacket( Packet p_packet )
{
	WelcomePacket data;
	data.unpack(p_packet);
	m_tcpClient->setId( data.clientNetworkIdentity );

	/************************************************************************/
	/* Debug info!															*/
	/************************************************************************/
	AntTweakBarWrapper::getInstance()->addReadOnlyVariable( 
		AntTweakBarWrapper::NETWORK,
		"NetId", TwType::TW_TYPE_INT32,
		m_tcpClient->getIdPointer(), "" );
}

void ClientPacketHandlerSystem::handleEntityCreationPacket(EntityCreationPacket p_packet)
{
	/************************************************************************/
	/* Mainly assemblage things!											*/
	/************************************************************************/
	Entity* entity;
	Component* component;
	if (p_packet.entityType == (char)EntityType::Ship )
	{
		int shipMeshId = static_cast<GraphicsBackendSystem*>(m_world->getSystem(
			SystemType::GraphicsBackendSystem ))->getMeshId("Ship.agl");

		/************************************************************************/
		/* This ship creation code have to be located somewhere else.			*/
		/************************************************************************/
		entity = m_world->createEntity();

		Transform* transform = new Transform( p_packet.translation, p_packet.rotation, 
			p_packet.scale);

		component = new RenderInfo( shipMeshId );
		entity->addComponent( ComponentType::RenderInfo, component );
		entity->addComponent( ComponentType::Transform, transform );		
		entity->addComponent(ComponentType::NetworkSynced,
			new NetworkSynced(p_packet.networkIdentity, p_packet.owner, EntityType::Ship));

		/************************************************************************/
		/* Check if the owner is the same as this client.						*/
		/************************************************************************/
		if(m_tcpClient->getId() == p_packet.owner)
		{
			component = new ShipFlyController(5.0f, 50.0f);
			entity->addComponent( ComponentType::ShipFlyController, component );

			component = new ShipEditController();
			entity->addComponent( ComponentType::ShipEditController, component);

			entity->addTag(ComponentType::TAG_ShipFlyMode, new ShipFlyMode_TAG());

			ConnectionPointSet* connectionPointSet = new ConnectionPointSet();
			connectionPointSet->m_connectionPoints.push_back(ConnectionPoint(
				AglMatrix::createTranslationMatrix(AglVector3(2.5f, 0, 0))));
			connectionPointSet->m_connectionPoints.push_back(ConnectionPoint(
				AglMatrix::createTranslationMatrix(AglVector3(-2.5f, 0, 0))));
			connectionPointSet->m_connectionPoints.push_back(ConnectionPoint(
				AglMatrix::createTranslationMatrix(AglVector3(0, 2.5f, 0))));

			entity->addComponent(ComponentType::ConnectionPointSet, connectionPointSet);
		}

		/************************************************************************/
		/* HACK: Score should probably be located in another entity.			*/
		/************************************************************************/
		//component = new PlayerScore();
		//entity->addComponent( ComponentType::PlayerScore, component );
		m_world->addEntity(entity);

		/************************************************************************/
		/* Attach a camera if it's the clients ship!							*/
		/************************************************************************/
		if(p_packet.owner == m_tcpClient->getId())
		{
			int shipId = entity->getIndex();
			float aspectRatio = 
				static_cast<GraphicsBackendSystem*>(m_world->getSystem(
				SystemType::GraphicsBackendSystem ))->getAspectRatio();

			entity = m_world->createEntity();
			component = new CameraInfo( aspectRatio );
			entity->addComponent( ComponentType::CameraInfo, component );
			component = new MainCamera();
			entity->addComponent( ComponentType::MainCamera, component );
			//component = new Input();
			//entity->addComponent( ComponentType::Input, component );
			component = new Transform( -5.0f, 0.0f, -5.0f );
			entity->addComponent( ComponentType::Transform, component );
			entity->addComponent( ComponentType::LookAtEntity, component );
			component = new LookAtEntity(shipId, 
				AglVector3(0,3,-10),
				AglQuaternion::identity(),
				10.0f,
				10.0f,
				10.0f);
			entity->addComponent( ComponentType::LookAtEntity, component );
			// default tag is follow
			entity->addTag(ComponentType::TAG_LookAtFollowMode, new LookAtFollowMode_TAG() );
			entity->addComponent(ComponentType::PlayerCameraController, new PlayerCameraController() );
			// listener
			component = new AudioListener();
			entity->addComponent(ComponentType::AudioListener, component);

			//Add a picking ray to the camera so that edit mode can be performed
			entity->addComponent(ComponentType::PickComponent, new PickComponent());

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
	else if ( p_packet.entityType == (char)EntityType::StaticProp )
	{
		int meshId = static_cast<GraphicsBackendSystem*>(m_world->getSystem(
			SystemType::GraphicsBackendSystem ))->getMeshId("P_cube");

		entity = m_world->createEntity();
		component = new Transform(p_packet.translation, p_packet.rotation, p_packet.scale);
		entity->addComponent( ComponentType::Transform, component );
		component = new RenderInfo(meshId);
		entity->addComponent(ComponentType::RenderInfo, component);

		m_world->addEntity(entity);
	}
	else if ( p_packet.entityType == (char)EntityType::ShipModule)
	{
		int meshId = static_cast<GraphicsBackendSystem*>(m_world->getSystem(
			SystemType::GraphicsBackendSystem ))->getMeshId("P_cube");

		if (p_packet.meshInfo == 1)
			meshId = static_cast<GraphicsBackendSystem*>(m_world->getSystem(
				SystemType::GraphicsBackendSystem ))->getMeshId("P_sphere");

		entity = m_world->createEntity();
		component = new Transform(p_packet.translation, p_packet.rotation, p_packet.scale);
		entity->addComponent( ComponentType::Transform, component );
		component = new RenderInfo(meshId);
		entity->addComponent(ComponentType::RenderInfo, component);

		entity->addComponent(ComponentType::NetworkSynced,
			new NetworkSynced(p_packet.networkIdentity, p_packet.owner, EntityType::ShipModule));

		m_world->addEntity(entity);
	}
	else
	{
		DEBUGPRINT(("Network Warning: Received unkown entity type from server!\n"));
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

	if(static_cast<TimerSystem*>(m_world->getSystem(SystemType::TimerSystem))->
		checkTimeInterval(TimerIntervals::EverySecond))
	{
		m_dataSentPerSecond = m_dataSentCounter;
		m_dataReceivedPerSecond = m_dataReceivedCounter;

		m_dataSentCounter = 0;
		m_dataReceivedCounter = 0;
	}
}
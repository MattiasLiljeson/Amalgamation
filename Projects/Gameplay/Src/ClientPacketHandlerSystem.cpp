#include "ClientPacketHandlerSystem.h"
#include "NetSyncedPlayerScoreTrackerSystem.h"
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
#include "Control.h"
#include "EntityType.h"
#include "PacketType.h"
#include "PickComponent.h"
#include "ParticleSystemEmitter.h"
#include "ShipEditController.h"
#include "ConnectionPointSet.h"
#include "TimerSystem.h"
#include "PingPacket.h"
#include "PongPacket.h"
#include "EntityUpdatePacket.h"
#include "ParticleUpdatePacket.h"
#include "EntityCreationPacket.h"
#include "WelcomePacket.h"
#include "UpdateClientStatsPacket.h"
#include "Extrapolate.h"
#include "InputBackendSystem.h"
#include "ParticleRenderSystem.h"
#include "AudioInfo.h"
#include <BasicSoundCreationInfo.h>
#include <PositionalSoundCreationInfo.h>
#include "AudioBackendSystem.h"
#include "PositionalSoundSource.h"
#include "SpawnSoundEffectPacket.h"
#include "NetsyncDirectMapperSystem.h"

// Debug
#include <DebugUtil.h>
#include <ToString.h>
#include "LightSources.h"
#include "LightsComponent.h"

ClientPacketHandlerSystem::ClientPacketHandlerSystem( TcpClient* p_tcpClient )
	: EntitySystem( SystemType::ClientPacketHandlerSystem, 1, 
					ComponentType::NetworkSynced)
{
	m_tcpClient = p_tcpClient;

	m_currentPing = 0;

	m_totalNetworkSynced = 0;
	m_numberOfSentPackets = 0;
	m_numberOfReceivedPackets = 0;
	m_totalDataSent = 0;
	m_totalDataReceived = 0;
	m_dataSentPerSecond = 0;
	m_dataReceivedPerSecond = 0;
	m_dataSentCounter = 0;
	m_dataReceivedCounter = 0;
	m_totalNumberOfOverflowPackets = 0;
	m_totalNumberOfStaticPropPacketsReceived = 0;
	m_lastBroadcastPacketIdentifier = 0;
	m_totalBroadcastPacketLost = 0;
}

ClientPacketHandlerSystem::~ClientPacketHandlerSystem()
{

}

void ClientPacketHandlerSystem::processEntities( const vector<Entity*>& p_entities )
{
	updateInitialPacketLossDebugData();
	updateCounters();
	m_totalNetworkSynced = p_entities.size();

	while (m_tcpClient->hasNewPackets())
	{
		Packet packet = m_tcpClient->popNewPacket();

		updateBroadcastPacketLossDebugData( packet.getUniquePacketIdentifier() );

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
				NetsyncDirectMapperSystem* directMapper =
					static_cast<NetsyncDirectMapperSystem*>(m_world->getSystem(
					SystemType::NetsyncDirectMapperSystem));
				Entity* entity = directMapper->getEntity( data.networkIdentity );
				if(entity != NULL)
				{
					Transform* transform = NULL;
					transform = static_cast<Transform*>(
						m_world->getComponentManager()->getComponent(
						entity->getIndex(), ComponentType::Transform ) );
					transform->setTranslation( data.translation );
					transform->setRotation( data.rotation );
					transform->setScale( data.scale );

					Extrapolate* extrapolate = NULL;
					extrapolate = static_cast<Extrapolate*>(
						entity->getComponent(ComponentType::Extrapolate) );
					extrapolate->serverUpdateTimeStamp = data.timestamp;
					extrapolate->velocityVector = data.velocity;
					extrapolate->angularVelocity = data.angularVelocity;
				}

			}
		}
		else if (packetType == (char)PacketType::ParticleUpdate)
		{			
			ParticleUpdatePacket data;
			data.unpack(packet);
			ParticleRenderSystem* gfx = static_cast<ParticleRenderSystem*>(m_world->getSystem(
				SystemType::ParticleRenderSystem ));
			AglParticleSystem* ps = gfx->getParticleSystem(data.networkIdentity);

			ps->setSpawnPoint(data.position);
			ps->setSpawnDirection(data.direction);
			ps->setSpawnSpeed(data.speed);
			ps->setSpawnFrequency(data.spawnFrequency);
		}
#pragma endregion 
		else if(packetType == (char)PacketType::SpawnSoundEffect)
		{
			SpawnSoundEffectPacket spawnSoundPacket;
			spawnSoundPacket.unpack( packet );
			if( spawnSoundPacket.positional &&
				spawnSoundPacket.attachedToNetsyncEntity == -1 )
			{
				// Short positional sound effect.
				AudioBackendSystem* audioBackend = static_cast<AudioBackendSystem*>(
					m_world->getSystem(SystemType::AudioBackendSystem));
				audioBackend->playPositionalSoundEffect(TESTSOUNDEFFECTPATH,
					SpawnSoundEffectPacket::soundEffectMapper[spawnSoundPacket.soundIdentifier],
					spawnSoundPacket.position);
			}
			else if( spawnSoundPacket.positional &&
				spawnSoundPacket.attachedToNetsyncEntity != -1 )
			{
				NetsyncDirectMapperSystem* directMapper =
					static_cast<NetsyncDirectMapperSystem*>(m_world->getSystem(
					SystemType::NetsyncDirectMapperSystem));
				Entity* entity = directMapper->getEntity(
					spawnSoundPacket.attachedToNetsyncEntity );
				if( entity != NULL )
				{
					entity->addComponent(ComponentType::PositionalSoundSource,
						new PositionalSoundSource(TESTSOUNDEFFECTPATH,
						SpawnSoundEffectPacket::soundEffectMapper[spawnSoundPacket.soundIdentifier],
						true, 1.0f));
				}
			}
			else
			{
				// Ambient sound effect.
			}
		}
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
			// Client ping
			m_currentPing = updateClientPacket.ping;
			float serverTimeAhead = updateClientPacket.currentServerTimestamp -
				m_world->getElapsedTime() + m_currentPing / 2.0f;
			m_tcpClient->setServerTimeAhead( serverTimeAhead );
			m_tcpClient->setPingToServer( m_currentPing );

			// Clients score
			NetSyncedPlayerScoreTrackerSystem* netSyncScoreTracker = static_cast<
				NetSyncedPlayerScoreTrackerSystem*>(m_world->getSystem(
				SystemType::NetSyncedPlayerScoreTrackerSystem));
			vector<Entity*> netSyncScoreEntities = netSyncScoreTracker->getNetScoreEntities();
			for(int playerId=0; playerId<updateClientPacket.MAXPLAYERS; playerId++)
			{
				for(unsigned int i=0; i<netSyncScoreEntities.size(); i++)
				{
					NetworkSynced* netSync = static_cast<NetworkSynced*>(
						netSyncScoreEntities[i]->getComponent(
						ComponentType::NetworkSynced));
					PlayerScore* playerScore = static_cast<PlayerScore*>(
						netSyncScoreEntities[i]->getComponent(
						ComponentType::PlayerScore));
					if(netSync->getNetworkOwner() ==
						updateClientPacket.playerIdentities[playerId])
					{
						playerScore->setModuleScore(updateClientPacket.scores[playerId]);
					}
				}
			}
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
		entity->addComponent( ComponentType::Extrapolate, new Extrapolate() );
		
		LightsComponent* lightComp = new LightsComponent();
		Light floodLight;
		float range = 100.0f;
		AglMatrix::componentsToMatrix(
			floodLight.offsetMat,
			AglVector3( range, range, range*20 ),
			AglQuaternion::constructFromAxisAndAngle( AglVector3( .0f, .0f, .0f), .0f ),
			AglVector3( 2.0f, 0.0f, 0.0f )
			);
		floodLight.instanceData.range = range*20;
		floodLight.instanceData.attenuation[1] = 0.1f;
		floodLight.instanceData.spotPower = 8.0f;
		floodLight.instanceData.lightDir[0] = 0.0f;
		floodLight.instanceData.lightDir[1] = 0.0f;
		floodLight.instanceData.lightDir[2] = 1.0f;
		floodLight.instanceData.diffuse[0] = 0.0f;
		floodLight.instanceData.diffuse[1] = 1.0f;
		floodLight.instanceData.diffuse[2] = 0.0f;
		//floodLight.instanceData.specular[0] = 2.0f;
		//floodLight.instanceData.specular[1] = 2.0f;
		//floodLight.instanceData.specular[2] = 2.0f;
		floodLight.instanceData.enabled = true;
		floodLight.instanceData.type = LightTypes::E_LightTypes_SPOT;

		lightComp->addLight( floodLight );
		AglMatrix::componentsToMatrix(
			floodLight.offsetMat,
			AglVector3( range, range, range*20 ),
			AglQuaternion::constructFromAxisAndAngle( AglVector3( .0f, .0f, .0f), .0f ),
			AglVector3( -2.0f, 0.0f, 0.0f )
			);
		floodLight.instanceData.diffuse[0] = 1.0f;
		floodLight.instanceData.diffuse[1] = 0.0f;
		floodLight.instanceData.diffuse[2] = 0.0f;
		lightComp->addLight( floodLight );

		entity->addComponent( ComponentType::LightsComponent, lightComp);

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
			// NOTE: (Johan) Moved the audio listener to the ship instead of the camera
			// because it was really weird to hear from the camera. This can of course
			// be changed back if game play fails in this way, but it's at least more
			// convenient for debugging!
			component = new AudioListener();
			entity->addComponent(ComponentType::AudioListener, component);
		}

		/************************************************************************/
		/* HACK: Score should probably be located in another entity.			*/
		/************************************************************************/
		component = new PlayerScore();
		entity->addComponent( ComponentType::PlayerScore, component );
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
			component = new Transform( -5.0f, 0.0f, -5.0f );
			entity->addComponent( ComponentType::Transform, component );
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
		m_totalNumberOfStaticPropPacketsReceived += 1;
		m_staticPropIdentities.push( p_packet.networkIdentity );
		int meshId = -1;

		GraphicsBackendSystem* gfxBackend = static_cast<GraphicsBackendSystem*>(
			m_world->getSystem( SystemType::GraphicsBackendSystem ));
		
		if (p_packet.isLevelProp)
			meshId = gfxBackend->getMeshId(m_levelPieceMapping.getModelFileName(p_packet.meshInfo));
		else	
		{
			meshId = static_cast<GraphicsBackendSystem*>(m_world->getSystem(
				SystemType::GraphicsBackendSystem ))->getMeshId("P_cube");

			if (p_packet.meshInfo == 1)
				meshId = static_cast<GraphicsBackendSystem*>(m_world->getSystem(
				SystemType::GraphicsBackendSystem ))->getMeshId("P_sphere");
		}
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
		entity->addComponent( ComponentType::Extrapolate, new Extrapolate() );

		m_world->addEntity(entity);
	}
	else if ( p_packet.entityType == (char)EntityType::ParticleSystem)
	{
		AglParticleSystemHeader h;
		if (p_packet.meshInfo == 0)
		{
			h.particleSize = AglVector2(2, 2);
			h.alignmentType = AglParticleSystemHeader::OBSERVER;
			h.spawnFrequency = 200;
			h.spawnSpeed = 5.0f;
			h.spread = 0.0f;
			h.fadeOutStart = 2.0f;
			h.fadeInStop = 0.0f;
			h.particleAge = 2;
			h.maxOpacity = 1.0f;
			h.color = AglVector4(0, 1, 0, 1.0f);
		}
		else
		{
			h.particleAge = 1;
			h.spawnSpeed = 0.02;
			h.spread = 1.0f;
			h.spawnFrequency = 200;
			h.color = AglVector4(0, 1.0f, 0.7f, 1.0f);
			h.fadeInStop = 0.5f;
			h.fadeOutStart = 0.5f;
			h.spawnOffset = 4.0f;
			h.maxOpacity = 0.5f;
			h.spawnOffsetType = AglParticleSystemHeader::ONSPHERE;
			h.particleSize = AglVector2(1.0f, 1.0f);
		}

		ParticleRenderSystem* gfx = static_cast<ParticleRenderSystem*>(m_world->getSystem(
			SystemType::ParticleRenderSystem ));
		//gfx->addParticleSystem();
		gfx->addParticleSystem(h, p_packet.networkIdentity);
	}
	else
	{
		DEBUGPRINT(("Network Warning: Received unknown entity type from server!\n"));
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

	AntTweakBarWrapper::getInstance()->addReadOnlyVariable(
		AntTweakBarWrapper::NETWORK, "Total lost broadcasts",
		TwType::TW_TYPE_UINT32, &m_totalBroadcastPacketLost, "group='Per frame'" );

	AntTweakBarWrapper::getInstance()->addReadOnlyVariable(
		AntTweakBarWrapper::getInstance()->BarType::NETWORK,
		"Total overflow packets", TwType::TW_TYPE_UINT32,
		&m_totalNumberOfOverflowPackets, "group='network bug'" );

	AntTweakBarWrapper::getInstance()->addReadOnlyVariable(
		AntTweakBarWrapper::getInstance()->BarType::NETWORK,
		"Total static props", TwType::TW_TYPE_UINT32,
		&m_totalNumberOfStaticPropPacketsReceived, "group='network bug'" );

	AntTweakBarWrapper::getInstance()->addReadOnlyVariable(
		AntTweakBarWrapper::getInstance()->BarType::NETWORK,
		"Total network synced", TwType::TW_TYPE_UINT32,
		&m_totalNetworkSynced, "group='network bug'" );
	

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

	m_totalNumberOfOverflowPackets = m_tcpClient->getTotalNumberOfOverflowPackets();

	if(static_cast<TimerSystem*>(m_world->getSystem(SystemType::TimerSystem))->
		checkTimeInterval(TimerIntervals::EverySecond))
	{
		m_dataSentPerSecond = m_dataSentCounter;
		m_dataReceivedPerSecond = m_dataReceivedCounter;

		m_dataSentCounter = 0;
		m_dataReceivedCounter = 0;
	}
}

void ClientPacketHandlerSystem::updateInitialPacketLossDebugData()
{
	if( static_cast<InputBackendSystem*>(m_world->getSystem(
		SystemType::InputBackendSystem))->getControlByEnum(
		InputHelper::KEY_0)->getDelta() > 0 )
	{
		if( m_staticPropIdentities.empty() )
		{
			DEBUGPRINT(( string(
				/* 0 - 511 */
				toString(0) + " - " +
				toString(511) +
				/* byte size */
				" = " +
				toString(511 * 51) +
				" bytes" +
				/* end */
				"\n").c_str() ));

				m_staticPropIdentitiesForAntTweakBar.push_back(
					pair<int, int>(0, 511));
		}

		if( !m_staticPropIdentities.empty() && m_staticPropIdentities.front() > 1 )
		{
			DEBUGPRINT(( string(
				/* 0 - x */
				toString(0) + " - " +
				toString(m_staticPropIdentities.front()) +
				/* byte size */
				" = " +
				toString((m_staticPropIdentities.front() + 1) * 51) +
				" bytes" +
				/* end */
				"\n").c_str() ));
			
				m_staticPropIdentitiesForAntTweakBar.push_back(
					pair<int, int>(0, m_staticPropIdentities.front()));
		}

		while( m_staticPropIdentities.size() >= 2 )
		{
			int firstValue = m_staticPropIdentities.front();
			m_staticPropIdentities.pop();
			int secondValue = m_staticPropIdentities.front();

			if( secondValue - firstValue > 1 )
			{
				DEBUGPRINT(( string(
					/* x - y */
					toString(firstValue) + " - " +
					toString(secondValue - 1) +
					/* byte size */
					" = " +
					toString((secondValue - firstValue) * 51) +
					" bytes" +
					/* end */
					"\n").c_str() ));

				m_staticPropIdentitiesForAntTweakBar.push_back(
					pair<int, int>(firstValue, secondValue));
			}
		}

		if( m_staticPropIdentities.size() == 1 )
		{
			if( m_staticPropIdentities.front() < 511 )
			{
				DEBUGPRINT(( string(
					/* x - 511 */
					toString(m_staticPropIdentities.front()) + " - " +
					toString(511) +
					/* byte size */
					" = " +
					toString((511 - m_staticPropIdentities.front()) * 51) +
					" bytes" +
					/* end */
					"\n").c_str() ));

				m_staticPropIdentitiesForAntTweakBar.push_back(
					pair<int, int>(m_staticPropIdentities.front(), 511));

					m_staticPropIdentities.pop();
			}
		}

		for( unsigned int i=0; i<m_staticPropIdentitiesForAntTweakBar.size(); i++ )
		{
			AntTweakBarWrapper::getInstance()->addReadOnlyVariable(
				AntTweakBarWrapper::NETWORK,
				("min" + toString(i)).c_str(), TwType::TW_TYPE_INT32,
				&m_staticPropIdentitiesForAntTweakBar[i].first,
				"group='Missing packets range'" );

			AntTweakBarWrapper::getInstance()->addReadOnlyVariable(
				AntTweakBarWrapper::NETWORK,
				("max" + toString(i)).c_str(), TwType::TW_TYPE_INT32,
				&m_staticPropIdentitiesForAntTweakBar[i].second,
				"group='Missing packets range'" );
		}
	}
}

void ClientPacketHandlerSystem::updateBroadcastPacketLossDebugData(
	unsigned int p_packetIdentifier )
{
	if( m_lastBroadcastPacketIdentifier == 0)
	{
		m_lastBroadcastPacketIdentifier = p_packetIdentifier;
	}
	else if( p_packetIdentifier > m_lastBroadcastPacketIdentifier + 1 )
	{
		m_totalBroadcastPacketLost += p_packetIdentifier -
			(m_lastBroadcastPacketIdentifier + 1);
		m_lastBroadcastPacketIdentifier = p_packetIdentifier;
	}
	else if( p_packetIdentifier == m_lastBroadcastPacketIdentifier + 1 )
	{
		m_lastBroadcastPacketIdentifier = p_packetIdentifier;
	}
}

#include "AudioListener.h"
#include "BodyInitData.h"
#include "ClientPacketHandlerSystem.h"
#include "GameStatsSystem.h"
#include "NetSyncedPlayerScoreTrackerSystem.h"
#include "PhysicsBody.h"
#include <AntTweakBarWrapper.h>
#include <Component.h>
#include <ComponentType.h>
#include <Entity.h>
#include <Packet.h>
#include <SystemType.h>
#include <TcpClient.h>
#include "LobbySystem.h"

// Components
#include "AudioBackendSystem.h"
#include "AudioInfo.h"
#include "CameraInfo.h"
#include "ConnectionPointSet.h"
#include "Control.h"
#include "EntityType.h"
#include "Extrapolate.h"
#include "GameplayTags.h"
#include "HudElement.h"
#include "Input.h"
#include "InputBackendSystem.h"
#include "InterpolationComponent.h"
#include "LookAtEntity.h"
#include "NetsyncDirectMapperSystem.h"
#include "NetworkSynced.h"
#include "PacketType.h"
#include "ParticleRenderSystem.h"
#include "ParticleSystemEmitter.h"
#include "PickComponent.h"
#include "PingPacket.h"
#include "PlayerCameraController.h"
#include "PlayerScore.h"
#include "PongPacket.h"
#include "PositionalSoundSource.h"
#include "RenderInfo.h"
#include "ShipEditController.h"
#include "ShipFlyController.h"
#include "TimerSystem.h"
#include "Transform.h"
#include "WelcomePacket.h"
#include <BasicSoundCreationInfo.h>
#include <PositionalSoundCreationInfo.h>
#include "OnHitScoreEffectPacket.h"
#include "ScoreWorldVisualizerSystem.h"
#include "ShipModule.h"

// Packets
#include "EntityCreationPacket.h"
#include "EntityDeletionPacket.h"
#include "EntityUpdatePacket.h"
#include "ParticleSystemCreationInfo.h"
#include "PlayersWinLosePacket.h"
#include "RemoveSoundEffectPacket.h"
#include "SpawnSoundEffectPacket.h"
#include "UpdateClientStatsPacket.h"
#include "ParticleUpdatePacket.h"
#include "ModuleTriggerPacket.h"
#include "ModuleStateChangePacket.h"
#include "NewlyConnectedPlayerPacket.h"

// Debug
#include "EntityFactory.h"
#include "LightsComponent.h"
#include "ParticleSystemCreationInfo.h"
#include "ParticleSystemsComponent.h"
#include "PlayersWinLosePacket.h"
#include "RemoveSoundEffectPacket.h"
#include <DebugUtil.h>
#include <ParticleSystemAndTexture.h>
#include <ToString.h>
#include "SlotParticleEffectPacket.h"
#include "ConnectionVisualizerSystem.h"

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

		if (packetType == (char)PacketType::EntityUpdate)
		{
			EntityUpdatePacket data;
			packet.ReadData(&data, sizeof(EntityUpdatePacket));

			if (data.entityType == (char)EntityType::EndBatch){
				handleBatch();
			}
			else
			{
				m_batch.push_back(data);
			}
		}

		else if( packetType == (char)PacketType::ParticleSystemCreationInfo)
		{
			ParticleSystemCreationInfo info;
			packet.ReadData( &info, sizeof(ParticleSystemCreationInfo) );
			handleParticleSystemCreation( info );
		}

		else if (packetType == (char)PacketType::ParticleUpdate)
		{
			ParticleUpdatePacket particleData;
			particleData.unpack( packet );
			handleParticleSystemUpdate( particleData );
			
		}
		else if (packetType == (char)PacketType::SlotParticleEffectPacket)
		{
			SlotParticleEffectPacket data;
			data.unpack(packet);

			if (data.networkIdentity < 0)
			{
				ConnectionVisualizerSystem* conVis = static_cast<ConnectionVisualizerSystem*>(m_world->getSystem(SystemType::ConnectionVisualizerSystem));
				conVis->disableAll();
			}
			else
			{
				NetsyncDirectMapperSystem* directMapper =
					static_cast<NetsyncDirectMapperSystem*>(m_world->getSystem(
					SystemType::NetsyncDirectMapperSystem));
				Entity* parent = directMapper->getEntity(data.networkIdentity);

				ConnectionVisualizerSystem* conVis = static_cast<ConnectionVisualizerSystem*>(m_world->getSystem(SystemType::ConnectionVisualizerSystem));
				conVis->addEffect(ConnectionVisualizerSystem::ConnectionEffectData(parent, data.slot, data.translationOffset, data.forwardDirection, !data.active));
			}
		}

		else if(packetType == (char)PacketType::SpawnSoundEffect)
		{
			AudioBackendSystem* audioBackend = static_cast<AudioBackendSystem*>(
				m_world->getSystem(SystemType::AudioBackendSystem));
			SpawnSoundEffectPacket spawnSoundPacket;
			spawnSoundPacket.unpack( packet );
			if( spawnSoundPacket.positional &&
				spawnSoundPacket.attachedToNetsyncEntity == -1 )
			{
				// Short positional sound effect.
				audioBackend->playPositionalSoundEffect(TESTSOUNDEFFECTPATH,
					SpawnSoundEffectPacket::soundEffectMapper[spawnSoundPacket.soundIdentifier],
					spawnSoundPacket.position);
			}
			else if( spawnSoundPacket.positional &&
				spawnSoundPacket.attachedToNetsyncEntity != -1 )
			{
				// Attached positional sound source.
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
					entity->applyComponentChanges();
				}
			}
			else if( !spawnSoundPacket.positional &&
				spawnSoundPacket.attachedToNetsyncEntity == -1 )
			{
				// Short ambient sound effect.
				// NOTE: (Johan) Seems to be a bug because only one sound effect will be played.
				audioBackend->playSoundEffect(TESTSOUNDEFFECTPATH,
					SpawnSoundEffectPacket::soundEffectMapper[spawnSoundPacket.soundIdentifier]);
			}
		}
		else if(packetType == (char)PacketType::RemoveSoundEffect)
		{
			RemoveSoundEffectPacket data;
			data.unpack( packet );
			NetsyncDirectMapperSystem* directMapper =
				static_cast<NetsyncDirectMapperSystem*>(m_world->getSystem(
				SystemType::NetsyncDirectMapperSystem));
			Entity* entity = directMapper->getEntity(data.attachedNetsyncIdentity);
			if( entity != NULL )
			{
				entity->removeComponent(ComponentType::PositionalSoundSource);
				entity->applyComponentChanges();
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

			// Update the game stats panel with name, ping, score.
			auto gameStats = static_cast<GameStatsSystem*>
								(m_world->getSystem(SystemType::GameStatsSystem));
			gameStats->updateStats(&updateClientPacket);

			// Client ping
			m_currentPing = updateClientPacket.ping[0];
			float serverTimeAhead = updateClientPacket.currentServerTimestamp -
				m_world->getElapsedTime() + m_currentPing / 2.0f;
			m_tcpClient->setServerTimeAhead( serverTimeAhead );
			m_tcpClient->setPingToServer( m_currentPing );

			// Clients score
			NetSyncedPlayerScoreTrackerSystem* netSyncScoreTracker = static_cast<
				NetSyncedPlayerScoreTrackerSystem*>(m_world->getSystem(
				SystemType::NetSyncedPlayerScoreTrackerSystem));
			vector<Entity*> netSyncScoreEntities = netSyncScoreTracker->getNetScoreEntities();
			for(int playerId=0; playerId<MAXPLAYERS; playerId++)
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
		else if(packetType == (char)PacketType::PlayerWinLose)
		{
			PlayersWinLosePacket winLose;
			winLose.unpack( packet );
			NetsyncDirectMapperSystem* directMapper =
				static_cast<NetsyncDirectMapperSystem*>(m_world->getSystem(
				SystemType::NetsyncDirectMapperSystem));
			for(int i=0; i<winLose.activePlayers; i++)
			{
				if(m_tcpClient->getId() == winLose.playerIdentities[i])
				{
					// NOTE: (Johan) This is where the winning/losing condition is read.
					// Use this later to print a nice text saying "Winner" or "Loser".
					if(winLose.winner[i])
					{
						MessageBoxA(NULL, "Winner!", "Warning!", MB_ICONWARNING);
					}
					else
					{
						MessageBoxA(NULL, "Loser!", "Warning!", MB_ICONWARNING);
					}
				}
			}
		}
		else if (packetType == (char)PacketType::OnHitScoreEffectPacket)
		{
			// number effect for score
			auto scoreVis = static_cast<ScoreWorldVisualizerSystem*>(
				m_world->getSystem(SystemType::ScoreWorldVisualizerSystem));

			if (scoreVis)
			{
				OnHitScoreEffectPacket scoreFx;
				scoreFx.unpack(packet);
				ScoreWorldVisualizerSystem::ScoreEffectCreationData inst;
				inst.score = scoreFx.score;
				inst.transform = AglMatrix(AglVector3::one(),scoreFx.angle,scoreFx.position);

				scoreVis->addEffect(inst);
			}
		}
		else if(packetType == (char)PacketType::EntityCreation)
		{
			EntityCreationPacket data;
			data.unpack(packet);
			handleEntityCreationPacket(data);
		}
		else if (packetType == (char)PacketType::EntityDeletion)
		{
			EntityDeletionPacket data;
			data.unpack(packet);
			handleEntityDeletionPacket(data);
		}
		else if(packetType == (char)PacketType::WelcomePacket)
		{
			handleWelcomePacket(packet);
		}
		else if(packetType == (char)PacketType::ModuleTriggerPacket)
		{
			ModuleTriggerPacket data;
			data.unpack(packet);
			Entity* moduleEntity = static_cast<NetsyncDirectMapperSystem*>(
				m_world->getSystem(SystemType::NetsyncDirectMapperSystem))->getEntity(
				data.moduleNetsyncIdentity);
			ShipModule* shipModule = static_cast<ShipModule*>(moduleEntity->getComponent(
				ComponentType::ShipModule));
			if(shipModule)
			{
				// Call client side activation/deactivation event.
				if(data.moduleTrigger)
				{
					shipModule->activate();
				}
				else
				{
					shipModule->deactivate();
				}
			}
		}
		else if(packetType == (char)PacketType::ModuleStateChangePacket){
			ModuleStateChangePacket data;
			data.unpack(packet);

			Entity* affectedModule = static_cast<NetsyncDirectMapperSystem*>(
				m_world->getSystem(SystemType::NetsyncDirectMapperSystem))->getEntity(
				data.affectedModule);

			if(affectedModule != NULL){
				ShipModule* shipModule = static_cast<ShipModule*>(
					affectedModule->getComponent(ComponentType::ShipModule));

				Entity* parrentObjec = static_cast<NetsyncDirectMapperSystem*>(
					m_world->getSystem(SystemType::NetsyncDirectMapperSystem))->getEntity(
					data.currentParrent);

				shipModule->m_parentEntity = parrentObjec->getIndex();
			}
			else{
				DEBUGWARNING(( "Unhandled module has changed!" ));
			}
			
		}
		else if(packetType == (char)PacketType::NewlyConnectedPlayerPacket)
		{
			NewlyConnectedPlayerPacket newlyConnected;
			newlyConnected.unpack(packet);
			static_cast<LobbySystem*>(m_world->getSystem(SystemType::LobbySystem))->
				addNewPlayer(newlyConnected);
		}
		else
		{
			DEBUGWARNING(( "Unhandled packet type!" ));
		}
	}
#pragma endregion
}

void ClientPacketHandlerSystem::handleWelcomePacket( Packet p_packet )
{
	WelcomePacket data;
	data.unpack(p_packet);
	m_tcpClient->setId( data.clientNetworkIdentity );
	m_tcpClient->setPlayerID( data.playerID );

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
	EntityFactory* factory = static_cast<EntityFactory*>
	(m_world->getSystem(SystemType::EntityFactory));
	if (p_packet.entityType != EntityType::DebugBox)
	{
		factory->entityFromPacket(p_packet);
	}
	else
	{
		Entity* entity = factory->entityFromRecipeOrFile( "DebugCube", "Assemblages/DebugCube.asd" );

		Component* component = new Transform(p_packet.translation, p_packet.rotation, p_packet.scale);
		entity->addComponent( ComponentType::Transform, component );
		entity->addComponent(ComponentType::NetworkSynced,
			new NetworkSynced(p_packet.networkIdentity, p_packet.owner, (EntityType::EntityEnums)p_packet.entityType));
		entity->addComponent(ComponentType::InterpolationComponent,new InterpolationComponent());
		m_world->addEntity(entity);
	}
}

void ClientPacketHandlerSystem::handleEntityDeletionPacket(EntityDeletionPacket p_packet)
{
	auto directMapper = static_cast<NetsyncDirectMapperSystem*>(
		m_world->getSystem(SystemType::NetsyncDirectMapperSystem));
	Entity* entity = directMapper->getEntity(p_packet.networkIdentity);
	m_world->deleteEntity(entity);
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
		InputHelper::KeyboardKeys_0)->getDelta() > 0 )
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
void ClientPacketHandlerSystem::handleBatch()
{
	for (unsigned int i = 0; i < m_batch.size(); i++)
	{
		EntityUpdatePacket data = m_batch[i];
		NetsyncDirectMapperSystem* directMapper =
			static_cast<NetsyncDirectMapperSystem*>(m_world->getSystem(
			SystemType::NetsyncDirectMapperSystem));
		Entity* entity = directMapper->getEntity( data.networkIdentity );
		if(entity != NULL)
		{
			Transform* transform = NULL;
			transform = static_cast<Transform*>(
				entity->getComponent( ComponentType::Transform ) );

			if( transform != NULL ) // Throw exception? /ML
			{
				/*InterpolationComponent* interpolation = NULL;
				Component* intcomp = m_world->getComponentManager()->getComponent(
					entity->getIndex(), ComponentType::InterpolationComponent );
				if (intcomp) interpolation = static_cast<InterpolationComponent*>(intcomp);
				if (interpolation )
				{
					// set up goal for queuing
					float handledTime = data.timestamp;
					InterpolationComponent::TransformGoal transformGoal;
					transformGoal.timestamp = handledTime;
					transformGoal.translation = data.translation;
					transformGoal.rotation = data.rotation;
					transformGoal.scale = data.scale;
					// enqueue data
					interpolation ->m_transformBuffer.push(transformGoal);
				}
				else*/
				{
					transform->setScale( data.scale );
					transform->setRotation( data.rotation );
					transform->setTranslation( data.translation );
				}
			}

			// Add extrapolation here if deemed necessary
		}
	}
	m_batch.clear();
}

void ClientPacketHandlerSystem::handleParticleSystemUpdate( const ParticleUpdatePacket& p_data )
{
	NetsyncDirectMapperSystem* directMapper = static_cast<NetsyncDirectMapperSystem*>
		( m_world->getSystem( SystemType::NetsyncDirectMapperSystem ) );

	Entity* entity = directMapper->getEntity( p_data.networkIdentity );
	if( entity != NULL )
	{
		//Transform* transform = NULL;

		ParticleSystemsComponent* particleComp = static_cast<ParticleSystemsComponent*>(
			entity->getComponent( ComponentType::ParticleSystemsComponent ) );

		if( particleComp != NULL )
		{
			int idx = p_data.particleSystemIdx;
			if( -1 < idx && idx < particleComp->getParticleSystemsPtr()->size() )
			{
				AglParticleSystem* particleSys = particleComp->getParticleSystemPtr(idx);
				if (particleSys)
				{
					particleSys->setSpawnPoint(		p_data.position);
					particleSys->setSpawnDirection(	p_data.direction);
					particleSys->setSpawnSpeed(		p_data.speed);
					particleSys->setSpawnFrequency(	p_data.spawnFrequency);
				}
			}
			else
			{
				// This should never happen as the particle systems should be synced.
				int breakHere = 1;
			}
		}
		else
		{
			// This should never happen as the entity should be correctly mapped.
			int breakHere = 1;
		}
	}
}

void ClientPacketHandlerSystem::handleParticleSystemCreation( const ParticleSystemCreationInfo& p_creationInfo )
{
	NetsyncDirectMapperSystem* directMapper = static_cast<NetsyncDirectMapperSystem*>
		( m_world->getSystem( SystemType::NetsyncDirectMapperSystem ) );

	int netID = p_creationInfo.entityNetId;
	Entity* entity = directMapper->getEntity(netID);

	ParticleSystemsComponent* particleComp = static_cast<ParticleSystemsComponent*>
		( entity->getComponent( ComponentType::ParticleSystemsComponent) );

	if( particleComp == NULL )
	{
		particleComp = new ParticleSystemsComponent();
		entity->addComponent( particleComp );
	}

	ParticleSystemInstruction instruction;
	instruction.textureFileName = p_creationInfo.textureFileName;
	instruction.particleSystem = AglParticleSystem(p_creationInfo.particleSysHeader);
	int psIdx = particleComp->addParticleSystemInstruction( instruction, p_creationInfo.particleSysIdx );
	if( psIdx != p_creationInfo.particleSysIdx )
	{
		// PARTICLE SYSTEMS NOT IN SYNC!
	}
}

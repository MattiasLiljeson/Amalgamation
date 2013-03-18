#include "ClientPacketHandlerSystem.h"

// Components
#include "AudioBackendSystem.h"
#include "AudioInfo.h"
#include "CameraInfo.h"
#include "ConnectionPointSet.h"
#include "Control.h"
#include "DamageComponent.h"
#include "EntityType.h"
#include "Extrapolate.h"
#include "GameplayTags.h"
#include "HudElement.h"
#include "Input.h"
#include "InputBackendSystem.h"
#include "InterpolationComponent.h"
#include "InterpolationComponent2.h"
#include "LightsComponent.h"
#include "LookAtEntity.h"
#include "NetsyncDirectMapperSystem.h"
#include "NetworkSynced.h"
#include "OnHitScoreEffectPacket.h"
#include "PacketType.h"
#include "ParticleRenderSystem.h"
#include "ParticleSystemEmitter.h"
#include "ParticleSystemsComponent.h"
#include "PickComponent.h"
#include "PingPacket.h"
#include "PlayerCameraController.h"
#include "PlayerComponent.h"
#include "PongPacket.h"
#include "SoundComponent.h"
#include "RenderInfo.h"
#include "ScoreWorldVisualizerSystem.h"
#include "ShipEditController.h"
#include "ShipFlyController.h"
#include "ShipModule.h"
#include "TimerSystem.h"
#include "Transform.h"
#include "WelcomePacket.h"
#include <BasicSoundCreationInfo.h>
#include <PositionalSoundCreationInfo.h>
#include "ShieldModule.h"


// Packets
#include "AnimationUpdatePacket.h"
#include "BombActivationPacket.h"
#include "ChangeStatePacket.h"
#include "DisconnectPacket.h"
#include "EditSphereUpdatePacket.h"
#include "EntityCreationPacket.h"
#include "EntityDeletionPacket.h"
#include "EntityUpdatePacket.h"
#include "HighlightEntityPacket.h"
#include "ModuleStateChangePacket.h"
#include "ModuleStatusEffectPacket.h"
#include "ModuleTriggerPacket.h"
#include "NewlyConnectedPlayerPacket.h"
#include "ParticleSystemCreationInfo.h"
#include "ParticleUpdatePacket.h"
#include "PlayersWinLosePacket.h"
#include "PlayersWinLosePacket.h"
#include "RemoveSoundEffectPacket.h"
#include "RemoveSoundEffectPacket.h"
#include "SelectionMarkerUpdatePacket.h"
#include "SlotParticleEffectPacket.h"
#include "SpawnExplosionPacket.h"
#include "SpawnSoundEffectPacket.h"
#include "UpdateClientStatsPacket.h"
#include "ShieldActivationPacket.h"

// Debug

// Misc
#include <DamageAccumulator.h>
#include <Cursor.h>

// Sort the following into their respective category. These have probably been auto-added.
#include "AnomalyBomb.h"
#include "AudioListener.h"
#include "BodyInitData.h"
#include "ClientStateSystem.h"
#include "ConnectionVisualizerSystem.h"
#include "EditSphereSystem.h"
#include "EntityFactory.h"
#include "GameStatsSystem.h"
#include "HudSystem.h"
#include "LobbySystem.h"
#include "ModuleStatusEffectSystem.h"
#include "NetSyncedPlayerScoreTrackerSystem.h"
#include "ParticleSystemCreationInfo.h"
#include "PhysicsBody.h"
#include "PlayerInfo.h"
#include "SelectionMarkerSystem.h"
#include "SkeletalAnimation.h"
#include <AntTweakBarWrapper.h>
#include <Component.h>
#include <ComponentType.h>
#include <DebugUtil.h>
#include <Entity.h>
#include <Packet.h>
#include <ParticleSystemAndTexture.h>
#include <SystemType.h>
#include <TcpClient.h>
#include <ToString.h>
#include "SlotMarkerSystem.h"
#include "TeslaHitPacket.h"
#include "TeslaEffectSystem.h"
#include "RootBoundingSpherePacket.h"
#include "LevelPieceRoot.h"
#include "PlayerSystem.h"
#include "ClientConnectToServerSystem.h"
#include "MenuSystem.h"
#include <OutputLogger.h>
#include "PlayerReadyPacket.h"
#include "libRocketBackendSystem.h"
#include "ModulesHighlightPacket.h"
#include "GlowAnimation.h"
#include "SettingsSystem.h"

ClientPacketHandlerSystem::ClientPacketHandlerSystem( TcpClient* p_tcpClient )
	: EntitySystem( SystemType::ClientPacketHandlerSystem, 1, 
	ComponentType::NetworkSynced)
{
	m_tcpClient = p_tcpClient;

	m_gameState = NULL;

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

	m_gameState = static_cast<ClientStateSystem*>(
		m_world->getSystem(SystemType::ClientStateSystem));

	handleServerDisconnect();

	switch (m_gameState->getCurrentState())
	{
	case GameStates::INGAME:
		handleIngameState();
		break;
	case GameStates::MENU:
		handleMenu();
		break;
	case GameStates::LOBBY:
		handleLobby();
		break;
	case GameStates::LOADING:
		handleLoading();
		break;
	case GameStates::FINISHEDLOADING:
		handleFinishedLoading();
		break;
	case GameStates::RESULTS:
		handleResults();
		break;
	default:
		break;
	}
}

void ClientPacketHandlerSystem::handleWelcomePacket( Packet p_packet )
{
	WelcomePacket data;
	data.unpack(p_packet);
	m_tcpClient->setServerGameTime( data.serverGameTime );
	m_tcpClient->setId( data.clientNetworkIdentity );
	m_tcpClient->setPlayerID( data.playerID );
	m_tcpClient->setServerName( data.serverName );

	auto settingsSystem = static_cast<SettingsSystem*>
		(m_world->getSystem(SystemType::SettingsSystem));

	settingsSystem->getSettingsRef()->defaultGameTime = data.serverGameTime;
	settingsSystem->getSettingsRef()->serverName = data.serverName;

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
		Entity* entity = factory->entityFromPacket(p_packet);
		if (entity)
		{
			Transform* transform = static_cast<Transform*>(entity->getComponent(
				ComponentType::Transform));
			if(transform)
			{
				transform->setTranslation(p_packet.translation);
				transform->setRotation(p_packet.rotation);
				transform->setScale(p_packet.scale);
			}
		}
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
	//if( static_cast<InputBackendSystem*>(m_world->getSystem(
	//	SystemType::InputBackendSystem))->getControlByEnum(
	//	InputHelper::KeyboardKeys_0)->getDelta() > 0 )
	//{
	//	if( m_staticPropIdentities.empty() )
	//	{
	//		DEBUGPRINT(( string(
	//			/* 0 - 511 */
	//			toString(0) + " - " +
	//			toString(511) +
	//			/* byte size */
	//			" = " +
	//			toString(511 * 51) +
	//			" bytes" +
	//			/* end */
	//			"\n").c_str() ));

	//			m_staticPropIdentitiesForAntTweakBar.push_back(
	//				pair<int, int>(0, 511));
	//	}

	//	if( !m_staticPropIdentities.empty() && m_staticPropIdentities.front() > 1 )
	//	{
	//		DEBUGPRINT(( string(
	//			/* 0 - x */
	//			toString(0) + " - " +
	//			toString(m_staticPropIdentities.front()) +
	//			/* byte size */
	//			" = " +
	//			toString((m_staticPropIdentities.front() + 1) * 51) +
	//			" bytes" +
	//			/* end */
	//			"\n").c_str() ));

	//			m_staticPropIdentitiesForAntTweakBar.push_back(
	//				pair<int, int>(0, m_staticPropIdentities.front()));
	//	}

	//	while( m_staticPropIdentities.size() >= 2 )
	//	{
	//		int firstValue = m_staticPropIdentities.front();
	//		m_staticPropIdentities.pop();
	//		int secondValue = m_staticPropIdentities.front();

	//		if( secondValue - firstValue > 1 )
	//		{
	//			DEBUGPRINT(( string(
	//				/* x - y */
	//				toString(firstValue) + " - " +
	//				toString(secondValue - 1) +
	//				/* byte size */
	//				" = " +
	//				toString((secondValue - firstValue) * 51) +
	//				" bytes" +
	//				/* end */
	//				"\n").c_str() ));

	//			m_staticPropIdentitiesForAntTweakBar.push_back(
	//				pair<int, int>(firstValue, secondValue));
	//		}
	//	}

	//	if( m_staticPropIdentities.size() == 1 )
	//	{
	//		if( m_staticPropIdentities.front() < 511 )
	//		{
	//			DEBUGPRINT(( string(
	//				/* x - 511 */
	//				toString(m_staticPropIdentities.front()) + " - " +
	//				toString(511) +
	//				/* byte size */
	//				" = " +
	//				toString((511 - m_staticPropIdentities.front()) * 51) +
	//				" bytes" +
	//				/* end */
	//				"\n").c_str() ));

	//			m_staticPropIdentitiesForAntTweakBar.push_back(
	//				pair<int, int>(m_staticPropIdentities.front(), 511));

	//				m_staticPropIdentities.pop();
	//		}
	//	}
	//	for( unsigned int i=0; i<m_staticPropIdentitiesForAntTweakBar.size(); i++ )
	//	{
	//		AntTweakBarWrapper::getInstance()->addReadOnlyVariable(
	//			AntTweakBarWrapper::NETWORK,
	//			("min" + toString(i)).c_str(), TwType::TW_TYPE_INT32,
	//			&m_staticPropIdentitiesForAntTweakBar[i].first,
	//			"group='Missing packets range'" );

	//		AntTweakBarWrapper::getInstance()->addReadOnlyVariable(
	//			AntTweakBarWrapper::NETWORK,
	//			("max" + toString(i)).c_str(), TwType::TW_TYPE_INT32,
	//			&m_staticPropIdentitiesForAntTweakBar[i].second,
	//			"group='Missing packets range'" );
	//	}

	//}
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

			int idx = static_cast<unsigned int>(p_data.particleSystemIdx);
			if( -1 < idx && idx < particleComp->getParticleSystemsPtr()->size() )
			{
				AglParticleSystem* particleSys = particleComp->getParticleSystemPtr(idx);
				if (particleSys)
				{
					if (entity->getName() == "ClientMinigun")
					{
						int k = 0;
					}

					AglVector3 pos = p_data.position;
					if (particleSys->getParticleSpace() == AglParticleSystemHeader::AglSpace_SCREEN)
					{
						EntityManager* entitymanager = m_world->getEntityManager();
						Entity* cam = entitymanager->getFirstEntityByComponentType(ComponentType::TAG_MainCamera);
						CameraInfo* info = static_cast<CameraInfo*>(cam->getComponent(ComponentType::CameraInfo));
						Transform* transform = static_cast<Transform*>(
							cam->getComponent( ComponentType::ComponentTypeIdx::Transform ) );
						AglVector3 position = transform->getTranslation();
						AglQuaternion rotation = transform->getRotation();
						AglVector3 lookTarget = position+transform->getMatrix().GetForward();
						AglVector3 up = transform->getMatrix().GetUp();
						AglMatrix view = AglMatrix::createViewMatrix(position,
							lookTarget,
							up);

						AglVector4 pos4d(pos.x, pos.y, pos.z, 1.0f);
						pos4d.transform(view*info->m_projMat);
						pos4d /= pos4d.w;
						pos = AglVector3(pos4d.x, pos4d.y, 0);
					}


					particleSys->setSpawnPoint(		pos, p_data.forceParticleMove);
					particleSys->setSpawnDirection(	p_data.direction);
					particleSys->setSpawnSpeed(		p_data.speed);
					if (p_data.spawnFrequency != -1)
						particleSys->setSpawnFrequency(	p_data.spawnFrequency);
					else
					{
						particleSys->restart();
					}
					particleSys->setColor(p_data.color);
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

void ClientPacketHandlerSystem::handleEntitySounds(const SoundPacket& p_data)
{
	NetsyncDirectMapperSystem* directMapper = static_cast<NetsyncDirectMapperSystem*>
		( m_world->getSystem( SystemType::NetsyncDirectMapperSystem ) );

	Entity* entity = directMapper->getEntity( p_data.target );

	SoundComponent* sound = static_cast<SoundComponent*>(entity->getComponent(ComponentType::SoundComponent));
	sound->getSoundHeaderByIndex((AudioHeader::SoundType)p_data.soundType, p_data.targetSlot)->queuedPlayingState = (AudioHeader::PlayState)p_data.queuedPlayingState;
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

void ClientPacketHandlerSystem::printPacketTypeNotHandled( string p_stateName, int p_packetType )
{
	DEBUGPRINT((("Packet type not handled("+p_stateName+"): " +
		toString(p_packetType) + "\n").c_str()));
}

void ClientPacketHandlerSystem::handleIngameState()
{
	while (m_tcpClient->hasNewPackets())
	{
		//Packet packet = m_tcpClient->popNewPacket();
		Packet& packet = m_tcpClient->getFrontPacket();

		updateBroadcastPacketLossDebugData( packet.getUniquePacketIdentifier() );

		char packetType;

		packetType = packet.getPacketType();

		if (packetType == (char)PacketType::EntityUpdate)
		{
			EntityUpdatePacket data;
			packet.ReadData(&data, sizeof(EntityUpdatePacket));

			if (data.entityType == (char)EntityType::EndBatch){
				handleBatch(0);
			} else {
				m_batch.push_back(data);
			}
		}

		else if( packetType == (char)PacketType::ParticleSystemCreationInfo)
		{
			ParticleSystemCreationInfo info;
			packet.ReadData( &info, sizeof(ParticleSystemCreationInfo) );
			handleParticleSystemCreation( info );
		}
		else if (packetType == (char)PacketType::SoundPacket)
		{
			SoundPacket spacket;
			spacket.unpack(packet);
			handleEntitySounds(spacket);
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
				conVis->addEffect(ConnectionVisualizerSystem::ConnectionEffectData(parent, data.slot, data.translationOffset, data.forwardDirection, !data.active || !data.inEditMode));

				if (!data.active)
				{
					//Add an icon
					SlotMarkerSystem* slotmarker = static_cast<SlotMarkerSystem*>(m_world->getSystem(SystemType::SlotMarkerSystem));
					EntityType type;
					type.type = (EntityType::EntityEnums)data.moduleType;
					slotmarker->addMarker(data.shipSlot, type);
				}
				else if (data.networkIdentity >= 0)
				{
					//Remove an icon
					SlotMarkerSystem* slotmarker = static_cast<SlotMarkerSystem*>(m_world->getSystem(SystemType::SlotMarkerSystem));
					EntityType type;
					type.type = (EntityType::EntityEnums)data.moduleType;
					slotmarker->removeMarker(data.shipSlot, type);
				}

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
	//			audioBackend->playPositionalSoundEffect(TESTSOUNDEFFECTPATH,
	//				SpawnSoundEffectPacket::soundEffectMapper[spawnSoundPacket.soundIdentifier],
	//				spawnSoundPacket.position);
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
					// RM-RT 2013-03-04
					/*
					Component* positionalSound = entity->getComponent(
						ComponentType::PositionalSoundSource);
					if(positionalSound != NULL)
					{
						entity->addComponent(ComponentType::PositionalSoundSource,
							new PositionalSoundSource(TESTSOUNDEFFECTPATH,
							SpawnSoundEffectPacket::soundEffectMapper[spawnSoundPacket.soundIdentifier],
							true, 1.0f));
						entity->applyComponentChanges();
					}
					*/
				}
			}
			else if( !spawnSoundPacket.positional &&
				spawnSoundPacket.attachedToNetsyncEntity == -1 )
			{
				// Short ambient sound effect.
				// NOTE: (Johan) Seems to be a bug because only one sound effect will be played.
				//audioBackend->playSoundEffect(TESTSOUNDEFFECTPATH,
				//	SpawnSoundEffectPacket::soundEffectMapper[spawnSoundPacket.soundIdentifier]);
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
				entity->removeComponent(ComponentType::SoundComponent);
				entity->applyComponentChanges();
			}
		}
		else if(packetType == (char)PacketType::BombActivationPacket)
		{
			BombActivationPacket bombPacket;
			bombPacket.unpack(packet);
			NetsyncDirectMapperSystem* netsyncMapper = static_cast<
				NetsyncDirectMapperSystem*>(m_world->getSystem(
				SystemType::NetsyncDirectMapperSystem));
			Entity* bombEntity = netsyncMapper->getEntity(bombPacket.netsyncId);
			if(bombEntity != NULL)
			{
				AnomalyBomb* anomalyBomb = static_cast<AnomalyBomb*>(
					bombEntity->getComponent(ComponentType::AnomalyBomb));
				if(anomalyBomb != NULL)
				{
					anomalyBomb->activated = true;
					static_cast<EntityFactory*>(m_world->getSystem(
						SystemType::EntityFactory))->createAnomalyPieces(
						bombEntity->getIndex());
				}
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

			// Convert from seconds to milliseconds.
			m_currentPing = (totalElapsedTime - pongPacket.timeStamp)*1000.0f;
		}
		else if (packetType == (char)PacketType::ClientDisconnect)
		{
			DisconnectPacket dcPacket;
			dcPacket.unpack(packet);

			handlePlayerDisconnect(dcPacket);
		}
		else if(packetType == (char)PacketType::UpdateClientStats)
		{
			UpdateClientStatsPacket updateClientPacket;
			updateClientPacket.unpack(packet);

			// Update the game stats panel with name, ping, score.
			auto gameStats = static_cast<GameStatsSystem*>
				(m_world->getSystem(SystemType::GameStatsSystem));
			gameStats->updateStats(&updateClientPacket);

			auto playerSys = static_cast<PlayerSystem*>(m_world->getSystem(SystemType::PlayerSystem));
			for (unsigned int i = 0; i < updateClientPacket.activePlayers; i++)
			{
				// Find the right player
				auto playerComp = playerSys->findPlayerComponentFromPlayerID(updateClientPacket.playerIdentities[i]);
				if (playerComp)
				{
					if (playerComp->m_playerID == m_tcpClient->getPlayerID())
					{
						m_currentPing = updateClientPacket.ping[i];
						float serverTimeAhead = updateClientPacket.currentServerTimestamp -
							m_world->getElapsedTime() + m_currentPing / 2.0f;
						m_tcpClient->setServerTimeAhead( serverTimeAhead );
						m_tcpClient->setPingToServer( m_currentPing );

						// Update score in hud
						HudSystem* hud = static_cast<HudSystem*>(m_world->getSystem(SystemType::HudSystem));

						if(hud){
							hud->setHUDData(HudSystem::SCORE,
								toString(updateClientPacket.scores[i]).c_str());
							hud->setHUDData(HudSystem::TIME,
								toString(
								toString(updateClientPacket.minutesUntilEndOfRound) + ":" +
								toString(updateClientPacket.secondsUntilEndOfRound)
								).c_str());
						}

						break;
					}
				}
			}
		}
		else if(packetType == (char)PacketType::PlayerWinLose)
		{
			PlayersWinLosePacket winLose;
			winLose.unpack( packet );

			GameStatsSystem* gameStats = static_cast<GameStatsSystem*>
				(m_world->getSystem(SystemType::GameStatsSystem));

			gameStats->updateResults(&winLose);

			/*
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
			*/
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
		else if (packetType == (char)PacketType::ModuleStatusEffectPacket)
		{
			// get effect system for modules
			auto moduleFxVis = static_cast<ModuleStatusEffectSystem*>(
				m_world->getSystem(SystemType::ModuleStatusEffectSystem));

			if (moduleFxVis)
			{
				auto directMapper =
					static_cast<NetsyncDirectMapperSystem*>(m_world->getSystem(
					SystemType::NetsyncDirectMapperSystem));
				//
				ModuleStatusEffectPacket effectPacket;
				effectPacket.unpack(packet);
				Entity* entity = directMapper->getEntity(effectPacket.m_moduleNetworkId);
				if (effectPacket.m_statusType==ModuleStatusEffectPacket::SPAWNED)
				{
					ModuleStatusEffectSystem::ModuleSpawnEffect fx;
					fx.networkId = effectPacket.m_moduleNetworkId;
					moduleFxVis->setSpawnedEffect(fx);
				}
				else if (entity)
				{
					switch (effectPacket.m_statusType)
					{
					case ModuleStatusEffectPacket::UNUSEDMODULE_STATUS:
						{
							ModuleStatusEffectSystem::ModuleUnusedEffect fx;
							effectPacket.m_mode==0?fx.mode=true:fx.mode=false;
							fx.moduleEntity = entity;
							moduleFxVis->setUnusedModuleEffect(fx);
							break;
						}
					case ModuleStatusEffectPacket::HEALTH_STATUS:
						{
							ModuleStatusEffectSystem::ModuleHealthStatEffect fx;
							fx.moduleEntity = entity;
							fx.health = effectPacket.m_value;
							moduleFxVis->setHealthEffect(fx);
							break;
						}
					case ModuleStatusEffectPacket::FREEFLOAT_STATUS:
						{
							ModuleStatusEffectSystem::ModuleFreefloatEffect fx;
							effectPacket.m_mode==0?fx.mode=true:fx.mode=false;
							fx.moduleEntity = entity;
							moduleFxVis->setFreefloatEffect(fx);
							break;
						}
					case ModuleStatusEffectPacket::VALUE_STATUS:
					default:
						{
							ModuleStatusEffectSystem::ModuleValueStatEffect fx;
							fx.moduleEntity = entity;
							fx.value = effectPacket.m_value;
							moduleFxVis->setValueEffect(fx);
							break;
						}
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
		else if (packetType == (char)PacketType::EntityDeletion)
		{
			EntityDeletionPacket data;
			data.unpack(packet);
			handleEntityDeletionPacket(data);
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

			if(affectedModule != NULL)
			{
				ShipModule* shipModule = static_cast<ShipModule*>(
					affectedModule->getComponent(ComponentType::ShipModule));

				if (shipModule)
				{
					if(affectedModule)
					{
						if (data.currentParrent >= 0)
						{
							Entity* parrentObjec = static_cast<NetsyncDirectMapperSystem*>(
								m_world->getSystem(SystemType::NetsyncDirectMapperSystem))->getEntity(
								data.currentParrent);

							if (parrentObjec)
								shipModule->m_parentEntity = parrentObjec->getIndex();
						}
						else
						{
							shipModule->m_parentEntity = -1;
						}
					}
				}
			}
			else{
				DEBUGWARNING(( "Unhandled module has changed!" ));
			}

		}
		else if (packetType == (char)PacketType::SpawnExplosionPacket)
		{
			SpawnExplosionPacket data;
			data.unpack(packet);
			EntityFactory* factory = static_cast<EntityFactory*>
				(m_world->getSystem(SystemType::EntityFactory));
			factory->createExplosion(data);
		}
		else if (packetType == (char)PacketType::AnimationUpdatePacket)
		{
			AnimationUpdatePacket data;
			data.unpack(packet);

			Entity* entity = static_cast<NetsyncDirectMapperSystem*>(
				m_world->getSystem(SystemType::NetsyncDirectMapperSystem))->getEntity(
				data.networkIdentity);

			SkeletalAnimation* anim = static_cast<SkeletalAnimation*>(entity->getComponent(ComponentType::SkeletalAnimation));
			anim->m_isPlaying = data.shouldPlay;

			int take = -1;
			for (unsigned int i = 0; i < anim->m_takes.size(); i++)
			{
				if (data.take == anim->m_takes[i].name)
					take = i;
			}
			if (data.shouldQueue)
				anim->queuedTakes.push_back(take);
			else
			{
				anim->m_currentTake = take;
				anim->m_time = anim->m_takes[anim->m_currentTake].startFrame / anim->m_fps;
			}

		}
		else if (packetType == (char)PacketType::EditSphereUpdatePacket)
		{
			EditSphereUpdatePacket update;
			update.unpack(packet);
			EditSphereSystem* editsystem = static_cast<EditSphereSystem*>
				(m_world->getSystem(SystemType::EditSphereSystem));
			editsystem->setSphere(update.m_offset, update.m_radius);
		}
		else if (packetType == (char)PacketType::SelectionMarkerUpdatePacket)
		{
			SelectionMarkerUpdatePacket update;
			update.unpack(packet);
			int ind = -1;
			if (update.targetNetworkIdentity >= 0)
			{
				Entity* entity = static_cast<NetsyncDirectMapperSystem*>(
					m_world->getSystem(SystemType::NetsyncDirectMapperSystem))->getEntity(
					update.targetNetworkIdentity);
				ind = entity->getIndex();
			}

			SelectionMarkerSystem* sys = static_cast<SelectionMarkerSystem*>
				(m_world->getSystem(SystemType::SelectionMarkerSystem));
			sys->setMarkerTarget(ind, update.transform);
		}
		else if (packetType == (char)PacketType::HighlightEntityPacket)
		{
			HighlightEntityPacket highlight;
			highlight.unpack(packet);
			if (highlight.target >= 0)
			{
				Entity* entity = static_cast<NetsyncDirectMapperSystem*>(
					m_world->getSystem(SystemType::NetsyncDirectMapperSystem))->getEntity(
					highlight.target);
				if (highlight.on)
				{
					if (!entity->getComponent(ComponentType::TAG_Highlight))
						entity->addComponent(ComponentType::TAG_Highlight, new Highlight_TAG());
				}
				else
				{
					entity->removeComponent(ComponentType::TAG_Highlight);
					entity->applyComponentChanges();
				}
			}
		}
		else if (packetType == (char)PacketType::ChangeStatePacket){
			ChangeStatePacket statePacket;
			statePacket.unpack(packet);

			if(statePacket.m_serverState == ServerStates::RESULTS ){
				m_gameState->setQueuedState(GameStates::RESULTS);
			}
		}
		else if( packetType == (char)PacketType::HitIndicatorPacket )
		{
			handleHitIndicationPacket( packet );
		}
		else if(packetType == (char)PacketType::TeslaHitPacket)
		{
			TeslaHitPacket hitPacket;
			hitPacket.unpack(packet);
			static_cast<TeslaEffectSystem*>(m_world->getSystem(
				SystemType::TeslaEffectSystem))->animateHits(hitPacket.identitySource,
				hitPacket.identitiesHit, hitPacket.numberOfHits,
				hitPacket.identitiesHitFloating, hitPacket.numberOfHitsFloating);
		}
		else if(packetType == (char)PacketType::ModulesHighlightPacket)
		{
			ModulesHighlightPacket data;
			data.unpack(packet);
			for(unsigned char i=0; i<data.numberOfHighlights; i++)
			{
				Entity* netModule = static_cast<NetsyncDirectMapperSystem*>(
					m_world->getSystem(SystemType::NetsyncDirectMapperSystem))->getEntity(
					data.modulesHighighted[i]);
				if(netModule)
				{
					GlowAnimation* glow = static_cast<GlowAnimation*>(
						netModule->getComponent(ComponentType::GlowAnimation));
					if(glow)
					{
						glow->resetAnimation();
					}
					else
					{
						netModule->addComponent(new GlowAnimation(AglVector4(0.1f, 0.3f, 0.1f, 1.0f), true, 0.75f));
						netModule->applyComponentChanges();
					}
				}
			}
		}
		else if(packetType == (char)PacketType::ShieldActivationPacket)
		{
			ShieldActivationPacket data;
			data.unpack(packet);
			Entity* netModule = static_cast<NetsyncDirectMapperSystem*>(
				m_world->getSystem(SystemType::NetsyncDirectMapperSystem))->getEntity(
				data.entityIndex);
			if(netModule)
			{
				ShieldModule* shieldModule = static_cast<ShieldModule*>(
					netModule->getComponent(ComponentType::ShieldModule));
				if(shieldModule)
				{
					if(data.shieldActivationState)
					{
						shieldModule->activation = 1.0f;
					}
					else
					{
						shieldModule->activation = 0;
					}
				}
			}
		}
		else
		{
			DEBUGWARNING(( toString("Unhandled packet type " + toString((int)packetType) + "!\n").c_str()));
		}

		// Pop packet!
		m_tcpClient->popFrontPacket();
	}
}

void ClientPacketHandlerSystem::handleMenu()
{
	while (m_tcpClient->hasNewPackets())
	{

		Packet packet = m_tcpClient->popNewPacket();
		//updateBroadcastPacketLossDebugData( packet.getUniquePacketIdentifier() );
		char packetType;
		packetType = packet.getPacketType();

		if(packetType == (char)PacketType::WelcomePacket)
		{
			handleWelcomePacket(packet);

			PlayerInfo playerInfoPacket;
			playerInfoPacket.playerName = m_tcpClient->getPlayerName();
			playerInfoPacket.playerID = m_tcpClient->getPlayerID();
			m_tcpClient->sendPacket(playerInfoPacket.pack());
			m_gameState->setQueuedState(GameStates::LOBBY);

			auto connectionSystem = static_cast<ClientConnectToServerSystem*>
				(m_world->getSystem(SystemType::ClientConnectoToServerSystem));

			auto gameSettings = static_cast<SettingsSystem*>
				(m_world->getSystem(SystemType::SettingsSystem));

			gameSettings->getSettingsRef()->playerName = m_tcpClient->getPlayerName();
			gameSettings->getSettingsRef()->ip = connectionSystem->getServerAddress();
			gameSettings->getSettingsRef()->port = connectionSystem->getServerPort();
		}	
		else
		{
			printPacketTypeNotHandled("Menu", (int)packetType);
		}
	}
}

void ClientPacketHandlerSystem::handleLobby()
{
	while (m_tcpClient->hasNewPackets())
	{

		Packet packet = m_tcpClient->popNewPacket();
		//updateBroadcastPacketLossDebugData( packet.getUniquePacketIdentifier() );
		char packetType;
		packetType = packet.getPacketType();

		if(packetType == (char)PacketType::NewlyConnectedPlayerPacket)
		{
			NewlyConnectedPlayerPacket newlyConnected;
			newlyConnected.unpack(packet);

			//Add entities here and utilize the player component 
			Entity* newPlayer = m_world->createEntity();
			newPlayer->setName("Player " + toString(newlyConnected.playerID));
			PlayerComponent* newPlayerComp = new PlayerComponent();
			newPlayerComp->m_networkID = newlyConnected.networkID;
			newPlayerComp->m_playerID = newlyConnected.playerID;
			newPlayerComp->m_ping = newlyConnected.ping;
			newPlayerComp->m_playerName = newlyConnected.playerName;
			newPlayerComp->setAbsoluteScore(newlyConnected.score);
			newPlayer->addComponent(newPlayerComp);
			m_world->addEntity(newPlayer);
			
			static_cast<LobbySystem*>(m_world->getSystem(SystemType::LobbySystem))->
				addNewPlayer(newlyConnected);

			// Reset all ready states.
			static_cast<LobbySystem*>(m_world->getSystem(SystemType::LobbySystem))
				->setAllPlayersReady(false);

			// Reset ready button
			auto rocketBackend = static_cast<LibRocketBackendSystem*>(
				m_world->getSystem(SystemType::LibRocketBackendSystem));
			int lobbyDocIdx = rocketBackend->getDocumentByName("lobby");
			rocketBackend->updateElement(lobbyDocIdx, "player_ready", "Ready");
		}
		else if (packetType == (char)PacketType::ClientDisconnect)
		{
			DisconnectPacket dcPacket;
			dcPacket.unpack(packet);

			// Reset all ready states.
			static_cast<LobbySystem*>(m_world->getSystem(SystemType::LobbySystem))
				->setAllPlayersReady(false);

			// Reset ready button
			auto rocketBackend = static_cast<LibRocketBackendSystem*>(
				m_world->getSystem(SystemType::LibRocketBackendSystem));
			int lobbyDocIdx = rocketBackend->getDocumentByName("lobby");
			rocketBackend->updateElement(lobbyDocIdx, "player_ready", "Ready");

			handlePlayerDisconnect(dcPacket);
		}
		else if (packetType == (char)PacketType::PlayerReadyPacket)
		{
			PlayerReadyPacket readyPacket;
			readyPacket.unpack(packet);
			
			static_cast<LobbySystem*>(m_world->getSystem(SystemType::LobbySystem))
				->setPlayerReady(readyPacket.playerId, readyPacket.ready);
			
			// If local player, then set unready/ready button
			if (readyPacket.playerId == m_tcpClient->getPlayerID())
			{
				auto rocketBackend = static_cast<LibRocketBackendSystem*>(
					m_world->getSystem(SystemType::LibRocketBackendSystem));

				int lobbyDocIdx = rocketBackend->getDocumentByName("lobby");
				if (readyPacket.ready)
				{
					rocketBackend->updateElement(lobbyDocIdx, "player_ready", "Unready");
				}
				else
				{
					rocketBackend->updateElement(lobbyDocIdx, "player_ready", "Ready");
				} 
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
		else if(packetType == (char)PacketType::ChangeStatePacket){
			ChangeStatePacket changeState;
			changeState.unpack(packet);

			if(changeState.m_serverState == ServerStates::LOADING){
				m_gameState->setQueuedState(GameStates::LOADING);
			}
		}
		else if(packetType == (char)PacketType::UpdateClientStats)
		{
			UpdateClientStatsPacket updateClientPacket;
			updateClientPacket.unpack(packet);

			auto lobbySys = static_cast<LobbySystem*>(m_world->getSystem(SystemType::LobbySystem));
			auto playerSys = static_cast<PlayerSystem*>(m_world->getSystem(SystemType::PlayerSystem));

			for (unsigned int i = 0; i < updateClientPacket.activePlayers; i++)
			{
				auto playerComp = playerSys->findPlayerComponentFromPlayerID(updateClientPacket.playerIdentities[i]);
				if (playerComp)
				{
					if (playerComp->m_playerID == m_tcpClient->getPlayerID())
					{
						m_currentPing = updateClientPacket.ping[i];
						float serverTimeAhead = updateClientPacket.currentServerTimestamp -
							m_world->getElapsedTime() + m_currentPing / 2.0f;
						m_tcpClient->setServerTimeAhead( serverTimeAhead );
						m_tcpClient->setPingToServer( m_currentPing );
					}
					lobbySys->updatePing(playerComp->m_playerID, updateClientPacket.ping[i]);
				}
			}
		}
		else if(packetType == (char)PacketType::EntityCreation){
			DEBUGWARNING(( "Server sent packets too fast and too furious!\n" ));
		}
		else
		{
			printPacketTypeNotHandled("Lobby", (int)packetType);
		}

	}
}

void ClientPacketHandlerSystem::handleLoading()
{
	if(m_gameState->getStateDelta(GameStates::LOADING) == EnumGameDelta::ENTEREDTHISFRAME){
		ChangeStatePacket changeState;
		changeState.m_gameState = GameStates::LOADING;
		m_tcpClient->sendPacket(changeState.pack());
	}
	while (m_tcpClient->hasNewPackets()){
		Packet packet = m_tcpClient->popNewPacket();
		//updateBroadcastPacketLossDebugData( packet.getUniquePacketIdentifier() );
		char packetType;
		packetType = packet.getPacketType();
		if(packetType == (char)PacketType::EntityCreation){
			EntityCreationPacket creation;
			creation.unpack(packet);
			handleEntityCreationPacket(creation);
		}

		else if(packetType == (char)PacketType::ChangeStatePacket){
			ChangeStatePacket changeState;
			changeState.unpack(packet);

			if(changeState.m_serverState == ServerStates::SENTALLPACKETS){
				m_gameState->setQueuedState(GameStates::FINISHEDLOADING);
			}
		}
		else if (packetType == (char)PacketType::ClientDisconnect)
		{
			DisconnectPacket dcPacket;
			dcPacket.unpack(packet);

			handlePlayerDisconnect(dcPacket);
		}
		else
		{
			printPacketTypeNotHandled("Loading", (int)packetType);
		}
	}
}
void ClientPacketHandlerSystem::handleBatch(int p_frame)
{
	float t = m_world->getElapsedTime();


	InputBackendSystem* input = static_cast<InputBackendSystem*>(m_world->getSystem(SystemType::InputBackendSystem));


	bool down = input->getStatusByEnum(InputHelper::KeyboardKeys_5) > 0;


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


				InterpolationComponent2* inter = static_cast<InterpolationComponent2*>(
					entity->getComponent( ComponentType::InterpolationComponent2 ) );


				CameraInfo* cam = static_cast<CameraInfo*>(
					entity->getComponent( ComponentType::CameraInfo ) );


				if (!inter)
				{
					//inter = new InterpolationComponent2();
					//entity->addComponent(ComponentType::InterpolationComponent2, inter);
					transform->setScale( data.scale );
					transform->setRotation( data.rotation );
					transform->setTranslation( data.translation );
					//inter->t = t;
					//entity->applyComponentChanges();
				}


				if (inter)
				{
					InterData interData;
					AglMatrix::componentsToMatrix(interData.transform, data.scale, data.rotation, data.translation);
					if (inter->data.size() > 0 && !down)
						interData.t = inter->data.back().t + (data.timestamp - inter->data.back().stamp);
					else
					{
						interData.t = t;
						inter->t = t;
					}
					interData.stamp = data.timestamp;
					inter->data.push_back(interData);
				}




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
					/*transform->setScale( data.scale );
					transform->setRotation( data.rotation );
					transform->setTranslation( data.translation );*/
				}
			}


			// Add extrapolation here if deemed necessary
		}
	}
	m_batch.clear();
}


void ClientPacketHandlerSystem::handleFinishedLoading()
{
	if(m_gameState->getStateDelta(GameStates::FINISHEDLOADING) ==
		EnumGameDelta::ENTEREDTHISFRAME)
	{
		ChangeStatePacket changeState;
		changeState.m_gameState = GameStates::FINISHEDLOADING;
		m_tcpClient->sendPacket(changeState.pack());
	}
	while (m_tcpClient->hasNewPackets())
	{

		Packet packet = m_tcpClient->popNewPacket();
		//updateBroadcastPacketLossDebugData( packet.getUniquePacketIdentifier() );
		char packetType;
		packetType = packet.getPacketType();

		if(packetType == (char)PacketType::ChangeStatePacket){
			ChangeStatePacket changeState;
			changeState.unpack(packet);

			if(changeState.m_serverState == ServerStates::INGAME){
				m_gameState->setQueuedState( GameStates::INGAME );

				auto* hudSystem = static_cast<HudSystem*>
					(m_world->getSystem(SystemType::HudSystem));
				hudSystem->setHUDData(HudSystem::PLAYERNAME,
					m_tcpClient->getPlayerName().c_str());
				hudSystem->setHUDData(HudSystem::SERVERNAME,"TheOnServ");
			}
		}
		else if (packetType == (char)PacketType::ClientDisconnect)
		{
			DisconnectPacket dcPacket;
			dcPacket.unpack(packet);

			handlePlayerDisconnect(dcPacket);
		}
		else
		{
			//printPacketTypeNotHandled("Finished Loading", (int)packetType);
		}
	}
}

void ClientPacketHandlerSystem::handleResults()
{
	if(m_gameState->getStateDelta(GameStates::RESULTS) ==
		EnumGameDelta::ENTEREDTHISFRAME)
	{
		//Notify the server that you have now successfully changed to the result state
		ChangeStatePacket changeState;
		changeState.m_gameState = GameStates::RESULTS;
		m_tcpClient->sendPacket(changeState.pack());
		
		auto inputBackend = static_cast<InputBackendSystem*>(m_world->getSystem(SystemType::InputBackendSystem));
		if (!inputBackend->getCursor()->isVisible())
			inputBackend->getCursor()->show();

		// If the current player is hosting the game, then request to shut it down!
		if (m_world->isHostingServer())
		{
			m_world->requestToQuitServer();
		}
	}
//	else
//	{
//		m_gameState->setQueuedState(GameStates::MENU);
//	}
}

void ClientPacketHandlerSystem::handleHitIndicationPacket( Packet& p_packet )
{
	const int OK = 0;
	const int ENTITY_NOT_FOUND = 1;
	const int COMPONENT_NOT_FOUND = 2;
	int status = OK;

	Entity* ship = m_world->getEntityManager()->
		getFirstEntityByComponentType( ComponentType::TAG_MyShip );

	if( ship == NULL ) {
		status = ENTITY_NOT_FOUND;
	}

	Component* comp = NULL;
	if( status == OK ) {
		comp = ship->getComponent( ComponentType::DamageComponent );
		if( comp == NULL ) {
			status = COMPONENT_NOT_FOUND;
		}
	}

	if ( status == OK ) {
		DamageAccumulator acc;
		p_packet.ReadData( &acc, sizeof(DamageAccumulator) ); 

		DamageComponent* hitComp = static_cast<DamageComponent*>( comp ); 
		int myNetworkId = static_cast<NetworkSynced*>(ship->getComponent( ComponentType::NetworkSynced ))->getNetworkOwner();

		if( acc.victim == myNetworkId ){
			hitComp->addDamage( acc );
		} else if ( acc.latestPerp == myNetworkId ) {
			hitComp->addHit( acc );
		}

	}
}

void ClientPacketHandlerSystem::resetFromDisconnect()
{
	// If this is the same player as the current client player, 
	// then the following should be done:
	// * Remove all players from the client!
	// * Clear lobby data!
	// * Enable the 'connect to server' system!
	// * Clear components from camera.
	// * Queue state to menu!
	//static_cast<PlayerSystem*>(m_world->getSystem(SystemType::PlayerSystem))->
	//	deleteAllPlayerEntities();
	//static_cast<LobbySystem*>(m_world->getSystem(SystemType::LobbySystem))->
	//	resetAllPlayers();
	//static_cast<ClientConnectToServerSystem*>(m_world->getSystem(SystemType::ClientConnectoToServerSystem))->
	//	setEnabled(true);
	//Entity* camera = m_world->getEntityManager()->getFirstEntityByComponentType(ComponentType::TAG_MainCamera);
	//if (camera)
	//{
	//	camera->removeComponent(ComponentType::NetworkSynced);
	//	camera->removeComponent(ComponentType::PlayerState);
	//	camera->removeComponent(ComponentType::PickComponent);
	//	camera->removeComponent(ComponentType::PlayerCameraController);
	//	camera->applyComponentChanges();
	//}

	//m_gameState->setQueuedState(GameStates::MENU);
}

void ClientPacketHandlerSystem::handleServerDisconnect()
{
	if (!m_tcpClient->hasActiveConnection() 
		&& m_gameState->getCurrentState() != GameStates::MENU
		&& m_gameState->getCurrentState() != GameStates::NONE
		&& m_gameState->getCurrentState() != GameStates::RESULTS)
	{
		static_cast<MenuSystem*>(m_world->getSystem(SystemType::MenuSystem))
			->displayDisconnectPopup();
		
		auto inputBackend = static_cast<InputBackendSystem*>(m_world->getSystem(SystemType::InputBackendSystem));
		if (!inputBackend->getCursor()->isVisible())
			inputBackend->getCursor()->show();
	}
}

void ClientPacketHandlerSystem::handlePlayerDisconnect( const DisconnectPacket& p_dcPacket )
{
	// If this is the same player as the current client player, 
	// then the following should be done:
	// * Remove all players from the client!
	// * Clear lobby data!
	// * Enable the 'connect to server' system!
	// * Queue state to menu!
	// This is done in 'reset from disconnect'!
	// Additionally 
	// * Disconnect from server immediately!
	if (p_dcPacket.clientNetworkIdentity == m_tcpClient->getId())
	{
		//resetFromDisconnect();
		m_tcpClient->disconnect();
		m_gameState->setQueuedState(GameStates::MENU);
	}
	// Else, the client player should:
	// * Remove player from lobby.
	// * Remove player from player system.
	else
	{
		static_cast<LobbySystem*>(m_world->getSystem(SystemType::LobbySystem))->
			removePlayer(p_dcPacket);
		static_cast<PlayerSystem*>(m_world->getSystem(SystemType::PlayerSystem))->
			deletePlayerEntity(p_dcPacket.playerID);
		m_world->getOutputLogger()->write(("Player " + toString(p_dcPacket.playerID) + " has left the game.\n").c_str());
	}

	// If the current player is hosting the game, then request to shut it down!
	if (m_world->isHostingServer())
	{
		m_world->requestToQuitServer();
	}

	// If this player is the host (id = 0) then request to shut down the server.
	//if (p_dcPacket.playerID == 0)
	//{
	//	//static_cast<ClientConnectToServerSystem*>(m_world->getSystem(SystemType::ClientConnectoToServerSystem))->
	//	//	setEnabled(true);
	//	m_world->requestToQuitServer();
	//}
}

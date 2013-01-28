#include "ServerUpdateSystem.h"
#include <TcpServer.h>
#include <Entity.h>
#include "TimerSystem.h"

#include "PacketType.h"
#include "NetworkSynced.h"
#include "Transform.h"
#include "EntityUpdatePacket.h"
#include "EntityDeletionPacket.h"
#include "PhysicsBody.h"
#include "PhysicsSystem.h"
#include "PhysicsController.h"
#include "ParticleUpdateData.h"
#include "ParticleUpdatePacket.h"
#include "UpdateClientStatsPacket.h"
#include "NetSyncedPlayerScoreTrackerSystem.h"
#include "PlayerScore.h"
#include "ServerClientInfoSystem.h"
#include "ClientInfo.h"

ServerUpdateSystem::ServerUpdateSystem( TcpServer* p_server )
	: EntitySystem( SystemType::NetworkUpdateSystem, 1, ComponentType::NetworkSynced )
{
	m_server = p_server;
}

ServerUpdateSystem::~ServerUpdateSystem()
{
}
	
void ServerUpdateSystem::processEntities( const vector<Entity*>& p_entities )
{
	NetworkSynced* netSync = NULL;
	Transform* transform = NULL;
	PhysicsBody* physicsBody = NULL;
	
	auto timerSys = static_cast<TimerSystem*>(m_world->getSystem(SystemType::TimerSystem));

	if( timerSys->checkTimeInterval(TimerIntervals::EverySecond) )
	{
		/************************************************************************/
		/* Send the client info to clients									    */
		/* TODO: This needs refactoring, as it is now currently uses a mixture  */
		/* of several components to construct a updateClientPacket and is		*/
		/* inefficiently made, I guess.											*/
		/* PlayerScore and NetSynced is never deleted from the server.			*/
		/* ClientInfo is, when a client is disconnecting. More should be		*/
		/* happening there.	// Alex												*/
		/************************************************************************/
		UpdateClientStatsPacket updatedClientPacket;

		// Add all players' ping to the packet
		auto clientInfoSys = static_cast<ServerClientInfoSystem*>(
			m_world->getSystem(SystemType::ServerClientInfoSystem));
		vector<Entity*> clientInfoEntities = clientInfoSys->getActiveEntities();
		// Set currently active players
		updatedClientPacket.activePlayers = clientInfoEntities.size();
		for (int i = 0; i < updatedClientPacket.activePlayers; i++)
		{
			auto clientInfo = static_cast<ClientInfo*>(
				clientInfoEntities[i]->getComponent(ComponentType::ClientInfo));

			updatedClientPacket.ping[i]				= clientInfo->ping;
			updatedClientPacket.playerIdentities[i]	= clientInfo->id;

			// Also add the players' score to the packet.
			auto netSyncedScoreSystem = static_cast<NetSyncedPlayerScoreTrackerSystem*>
				(m_world->getSystem(SystemType::NetSyncedPlayerScoreTrackerSystem));
			vector<Entity*> netSyncedScoreEntities =
				netSyncedScoreSystem->getNetScoreEntities();

			for (int j = 0; j < netSyncedScoreEntities.size(); j++)
			{
				PlayerScore* playerScore = static_cast<PlayerScore*>(
					netSyncedScoreEntities[j]->getComponent(ComponentType::PlayerScore));
				NetworkSynced* netSync = static_cast<NetworkSynced*>(
					netSyncedScoreEntities[j]->getComponent(ComponentType::NetworkSynced));

				if (clientInfo->id == netSync->getNetworkOwner())
				{
					updatedClientPacket.scores[i] = playerScore->getTotalScore();
					break; // NOTE: A break here, for the inner loop.
				}
			}
		}
		
		updatedClientPacket.currentServerTimestamp = m_world->getElapsedTime();
		// Also add the players' score to the packet.
		//NetSyncedPlayerScoreTrackerSystem* netSyncedScoreSystem =
		//	static_cast<NetSyncedPlayerScoreTrackerSystem*>(m_world->getSystem(
		//	SystemType::NetSyncedPlayerScoreTrackerSystem));

		//vector<Entity*> netSyncedScoreEntities =
		//	netSyncedScoreSystem->getNetScoreEntities();

		//int playerCount = 0;
		//for(unsigned int i=0; i<netSyncedScoreEntities.size(); i++)
		//{
		//	PlayerScore* playerScore = static_cast<PlayerScore*>(
		//		netSyncedScoreEntities[i]->getComponent(ComponentType::PlayerScore));
		//	NetworkSynced* netSync = static_cast<NetworkSynced*>(
		//		netSyncedScoreEntities[i]->getComponent(ComponentType::NetworkSynced));
		//	if(playerScore && netSync)
		//	{
		//		updatedClientPacket.playerIdentities[playerCount] =
		//			netSync->getNetworkOwner();
		//		// TODO: (Johan) Change score into whatever Anton sees fit, but for
		//		// now the score is an integer!
		//		updatedClientPacket.scores[playerCount] = playerScore->getTotalScore();
		//		playerCount += 1;
		//	}
		//}

		m_server->broadcastPacket(updatedClientPacket.pack());
		//m_server->unicastPacket(updatedClientPacket.pack(), packet.getSenderId());
	}

	else if( timerSys->checkTimeInterval(TimerIntervals::Every8Millisecond) )
	{
		for( unsigned int i=0; i<p_entities.size(); i++ )
		{

			netSync = static_cast<NetworkSynced*>(
				m_world->getComponentManager()->getComponent(
				p_entities[i]->getIndex(), ComponentType::NetworkSynced ) );

			if (netSync->getNetworkType() == EntityType::ParticleSystem)
			{
				ParticleUpdateData* data = static_cast<ParticleUpdateData*>(
					m_world->getComponentManager()->getComponent(
					p_entities[i]->getIndex(), ComponentType::ParticleUpdateData ) );

				ParticleUpdatePacket updatePacket;
				updatePacket.networkIdentity = netSync->getNetworkIdentity();
				updatePacket.position		= data->spawnPoint;
				updatePacket.direction		= data->direction;
				updatePacket.speed			= data->speed;
				updatePacket.spawnFrequency	= data->spawnFrequency;
				m_server->broadcastPacket( updatePacket.pack() );
			}
			else
			{
				transform = static_cast<Transform*>(
					m_world->getComponentManager()->getComponent(
					p_entities[i]->getIndex(), ComponentType::Transform ) );

				physicsBody = static_cast<PhysicsBody*>(
					p_entities[i]->getComponent(ComponentType::PhysicsBody));
				AglVector3 velocity = AglVector3();
				AglVector3 angularVelocity = AglVector3();
				if( physicsBody != NULL )	/* It is probably the ray entity that is
											 * missing the PhysicsBody component. */
				{
					PhysicsSystem* physicsSystem = static_cast<PhysicsSystem*>(
						m_world->getSystem(SystemType::PhysicsSystem));

					velocity = physicsSystem->getController()->getBody(
						physicsBody->m_id)->GetVelocity();

					angularVelocity = physicsSystem->getController()->getBody(
						physicsBody->m_id)->GetAngularVelocity();
				}

				EntityUpdatePacket updatePacket;
				updatePacket.networkIdentity = netSync->getNetworkIdentity();
				updatePacket.entityType		= static_cast<char>(netSync->getNetworkType());
				updatePacket.translation	= transform->getTranslation();
				updatePacket.rotation		= transform->getRotation();
				updatePacket.scale			= transform->getScale();
				updatePacket.timestamp		= m_world->getElapsedTime();
				updatePacket.velocity		= velocity;
				updatePacket.angularVelocity= angularVelocity;


				m_server->broadcastPacket( updatePacket.pack() );
			}
		}
	}
}

void ServerUpdateSystem::initialize()
{
}

void ServerUpdateSystem::removed( Entity* p_entity )
{
	EntityDeletionPacket packet;
	auto netSync = static_cast<NetworkSynced*>(
		p_entity->getComponent(ComponentType::NetworkSynced));
	
	packet.networkIdentity = netSync->getNetworkIdentity();
	m_server->broadcastPacket(packet.pack());
}

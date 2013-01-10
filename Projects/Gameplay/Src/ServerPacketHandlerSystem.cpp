#include "ServerPacketHandlerSystem.h"

// Components
#include "Transform.h"
#include "NetworkSynced.h"
#include "PhysicsBody.h"

// NetComm
#include <TcpServer.h>

#include "PacketType.h"
#include "EntityType.h"
#include "PhysicsSystem.h"

ServerPacketHandlerSystem::ServerPacketHandlerSystem( TcpServer* p_server )
	: EntitySystem( SystemType::ServerPacketHandlerSystem, 3,
	ComponentType::NetworkSynced, ComponentType::ShipFlyController,
	ComponentType::PhysicsBody )
{
	m_server = p_server;
}

ServerPacketHandlerSystem::~ServerPacketHandlerSystem()
{

}


void ServerPacketHandlerSystem::initialize()
{
	m_physics = static_cast<PhysicsSystem*>(
		m_world->getSystem( SystemType::PhysicsSystem ) );

	/************************************************************************/
	/* Determines the frequency on how often ping packets will be sent.		*/
	/************************************************************************/
	m_timerStartValue = m_timer = 0.5;
}

void ServerPacketHandlerSystem::processEntities( const vector<Entity*>& p_entities )
{
	while( m_server->hasNewPackets() )
	{
		Packet packet = m_server->popNewPacket();

		char packetType;
		packet >> packetType;

		if(packetType == (char)PacketType::PlayerInput)
		{
			char entityType;
			packet >> entityType;
			if(entityType == (char)EntityType::Ship)
			{
				AglVector3 thrustVec;
				AglVector3 angularVec;
				int networkId;

				packet >> thrustVec >> angularVec >> networkId;

				// Netsync networkId can be used to find an entity in O(1) instead of O(n)
				// Locate the entity using networkId by consulting the entitymanager in world
				//  entities[networkId]
				// world->getEntityManager->getEntity[networkId]
				Entity* entity = m_world->getEntityManager()->getEntity(networkId);

				if(entity)
				{
					PhysicsBody* physicsBody = NULL;
					physicsBody = static_cast<PhysicsBody*>(entity->getComponent(
						ComponentType::PhysicsBody ) );
					if( physicsBody )
					{
						m_physics->applyImpulse(physicsBody->m_id, thrustVec, angularVec);
					}
				}
			}
		}
		else if( packetType == (char)PacketType::Ping )
		{
			float clientTime;
			packet >> clientTime;

			Packet response((char)PacketType::Pong);
			response << clientTime;

			m_server->unicastPacket( response, packet.getSenderId() );
		}
		else if( packetType == (char)PacketType::Pong)
		{
			ClientInfo info;
			float totalElapsedTime = m_world->getElapsedTime();
			float timeWhenSent;

			packet >> timeWhenSent;

			/************************************************************************/
			/* Convert from seconds to milliseconds.								*/
			/************************************************************************/
			info.ping = (totalElapsedTime - timeWhenSent)*1000.0f;
			m_clients[packet.getSenderId()] = info;

			/************************************************************************/
			/* Send the "real" ping back to the client as a "your ping" message.    */
			/************************************************************************/
			Packet newClientStats((char)PacketType::UpdateClientStats);
			newClientStats << info.ping;
			m_server->unicastPacket(newClientStats, packet.getSenderId());
		}

		m_timer -= m_world->getDelta();
		if( m_timer <= 0 )
		{
			m_timer = m_timerStartValue;

			float timeStamp = m_world->getElapsedTime();

			Packet packet((char)PacketType::Ping);
			packet << timeStamp;

			m_server->broadcastPacket( packet );
		}
		
	}
}

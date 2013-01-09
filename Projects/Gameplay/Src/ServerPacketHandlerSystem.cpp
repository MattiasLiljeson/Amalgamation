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
#include <Windows.h>

ServerPacketHandlerSystem::ServerPacketHandlerSystem( TcpServer* p_server )
	: EntitySystem( SystemType::ServerPacketHandlerSystem, 3,
	ComponentType::NetworkSynced, ComponentType::ShipController,
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
			SYSTEMTIME timestamp;
			GetSystemTime( &timestamp );

			Packet response;
			response << (char)PacketType::Pong;
			response << timestamp;

			m_server->unicastPacket( response, packet.getSenderId() );
		}
		
	}
}

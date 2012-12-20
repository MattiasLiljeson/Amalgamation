#include "NetworkInputHandlerSystem.h"

// Components
#include "Transform.h"
#include "NetworkSynced.h"
#include "PhysicsBody.h"

// NetComm
#include <TcpServer.h>

#include "PacketType.h"
#include "NetworkType.h"
#include "PhysicsSystem.h"

NetworkInputHandlerSystem::NetworkInputHandlerSystem( TcpServer* p_server )
	: EntitySystem( SystemType::NetworkInputHandlerSystem, 3,
	ComponentType::NetworkSynced, ComponentType::ShipController,
	ComponentType::PhysicsBody )
{
	m_server = p_server;
}

NetworkInputHandlerSystem::~NetworkInputHandlerSystem()
{

}


void NetworkInputHandlerSystem::initialize()
{
	m_physics = static_cast<PhysicsSystem*>(
		m_world->getSystem( SystemType::PhysicsSystem ) );
}

void NetworkInputHandlerSystem::processEntities( const vector<Entity*>& p_entities )
{
	while( m_server->hasNewPackets() )
	{
		Packet packet = m_server->popNewPacket();

		char networkType;
		packet >> networkType;
		if( networkType == (char)NetworkType::Ship )
		{
			char packetType;
			packet >> packetType;
			if( packetType == (char)PacketType::PlayerInput )
			{
				AglVector3 thrustVec;
				AglVector3 angularVec;

				packet >> thrustVec >> angularVec;
				
				//cout << "ThrustVec(" << thrustVec.x << ", " <<
				//	thrustVec.y << ", " <<
				//	thrustVec.z <<"). ";
				//cout << "AngularVec(" << angularVec.x << ", " <<
				//	angularVec.y << ", " <<
				//	angularVec.z <<").\n";

				Entity* entity = NULL;

				// Netsync networkId can be used to find an entity in O(1) instead of O(n)
				// Locate the entity using networkId by consulting the entitymanager in world
				//  entities[networkId]
				// world->getEntityManager->getEntity[networkId]

				for( unsigned int i=0; i<p_entities.size(); i++ )
				{
					NetworkSynced* netSync = NULL;
					netSync = static_cast<NetworkSynced*>(p_entities[i]->getComponent(
						ComponentType::NetworkSynced ) );

					if(packet.getSenderId() == netSync->getNetworkOwner())
					{
						if( p_entities[i]->getComponent(ComponentType::ShipController) )
						{
							// Entity is controllable.
							entity = p_entities[i];
							break;
						}
					}
				}

				if(entity)
				{
					PhysicsBody* physicsBody = NULL;
					physicsBody = static_cast<PhysicsBody*>(entity->getComponent(
						ComponentType::PhysicsBody ) );
					if( physicsBody )
					{
						m_physics->applyImpulse(physicsBody->m_id, thrustVec, angularVec);
						//cout << physicsBody->m_id << endl;
					}
				}
			}
		}
	}
}

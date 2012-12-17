#include "NetworkInputHandlerSystem.h"

NetworkInputHandlerSystem::NetworkInputHandlerSystem( TcpServer* p_server )
	: EntitySystem( SystemType::NetworkInputHandlerSystem, 1,
	ComponentType::NetworkSynced )
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

				Entity* entity = NULL;

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
					}
				}
			}
		}
	}
}

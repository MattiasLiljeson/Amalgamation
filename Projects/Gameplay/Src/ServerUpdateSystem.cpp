#include "ServerUpdateSystem.h"
#include <TcpServer.h>
#include <Entity.h>
#include "TimerSystem.h"

#include "PacketType.h"
#include "NetworkSynced.h"
#include "Transform.h"
#include "EntityUpdatePacket.h"
#include "PhysicsBody.h"
#include "PhysicsSystem.h"

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
	
	if( static_cast<TimerSystem*>(m_world->getSystem(SystemType::TimerSystem))->
		checkTimeInterval(TimerIntervals::Every8Millisecond) )
	{
		for( unsigned int i=0; i<p_entities.size(); i++ )
		{

			netSync = static_cast<NetworkSynced*>(
				m_world->getComponentManager()->getComponent(
				p_entities[i]->getIndex(), ComponentType::NetworkSynced ) );

			if( netSync->getNetworkType() == EntityType::Ship ||
				netSync->getNetworkType() == EntityType::Prop ||
				netSync->getNetworkType() == EntityType::ShipModule)
			{
				transform = static_cast<Transform*>(
					m_world->getComponentManager()->getComponent(
					p_entities[i]->getIndex(), ComponentType::Transform ) );

				physicsBody = static_cast<PhysicsBody*>(
					p_entities[i]->getComponent(ComponentType::PhysicsBody));
				AglVector3 velocity = AglVector3(100.0f, 0, 0);
					//static_cast<PhysicsSystem*>(m_world->getSystem(
					//SystemType::PhysicsSystem))->getController()->getBody(physicsBody->m_id)->getVelocity();

				EntityUpdatePacket updatePacket;
				updatePacket.networkIdentity = netSync->getNetworkIdentity();
				updatePacket.entityType		= static_cast<char>(netSync->getNetworkType());
				updatePacket.translation	= transform->getTranslation();
				updatePacket.rotation		= transform->getRotation();
				updatePacket.scale			= transform->getScale();
				updatePacket.timestamp		= m_world->getElapsedTime();
				updatePacket.velocity		= velocity;


				m_server->broadcastPacket( updatePacket.pack() );
			}
		}
	}
}

void ServerUpdateSystem::initialize()
{
}
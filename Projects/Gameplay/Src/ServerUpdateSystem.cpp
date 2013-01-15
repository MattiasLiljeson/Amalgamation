#include "ServerUpdateSystem.h"
#include <TcpServer.h>
#include <Entity.h>
#include "TimerSystem.h"

#include "PacketType.h"
#include "NetworkSynced.h"
#include "Transform.h"
#include "EntityUpdatePacket.h"

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
	
	if( static_cast<TimerSystem*>(m_world->getSystem(SystemType::TimerSystem))->
		checkTimeInterval(TimerIntervals::Every8Millisecond) )
	{
		for( unsigned int i=0; i<p_entities.size(); i++ )
		{

			netSync = static_cast<NetworkSynced*>(
				m_world->getComponentManager()->getComponent(
				p_entities[i]->getIndex(), ComponentType::NetworkSynced ) );

			if( netSync->getNetworkType() == EntityType::Ship ||
				netSync->getNetworkType() == EntityType::Prop)
			{

				
				transform = static_cast<Transform*>(
					m_world->getComponentManager()->getComponent(
					p_entities[i]->getIndex(), ComponentType::Transform ) );

				EntityUpdatePacket updatePacket;
				updatePacket.networkIdentity = netSync->getNetworkIdentity();
				updatePacket.entityType		= static_cast<char>(netSync->getNetworkType());
				updatePacket.translation	= transform->getTranslation();
				updatePacket.rotation		= transform->getRotation();
				updatePacket.scale			= transform->getScale();

				m_server->broadcastPacket( updatePacket.pack() );
			}
		}
	}
}

void ServerUpdateSystem::initialize()
{
}
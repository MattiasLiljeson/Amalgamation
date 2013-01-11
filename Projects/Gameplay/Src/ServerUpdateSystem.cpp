#include "ServerUpdateSystem.h"
#include <TcpServer.h>
#include <Entity.h>

#include "PacketType.h"
#include "NetworkSynced.h"
#include "Transform.h"

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

			Packet updateEntityPacket( (char)PacketType::EntityUpdate );
			updateEntityPacket << (char)netSync->getNetworkType() 
				<< netSync->getNetworkIdentity() 
				<< transform->getTranslation() 
				<< transform->getRotation() 
				<< transform->getScale();

			m_server->broadcastPacket( updateEntityPacket );
		}
	}
}

void ServerUpdateSystem::initialize()
{
}
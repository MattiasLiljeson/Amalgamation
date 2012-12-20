#include "NetworkUpdateSystem.h"
#include <TcpServer.h>
#include <Entity.h>

#include "PacketType.h"
#include "NetworkSynced.h"
#include "Transform.h"

NetworkUpdateSystem::NetworkUpdateSystem( TcpServer* p_server )
	: EntitySystem( SystemType::NetworkUpdateSystem, 1, ComponentType::NetworkSynced )
{
	m_server = p_server;
}

NetworkUpdateSystem::~NetworkUpdateSystem()
{
}
	
void NetworkUpdateSystem::processEntities( const vector<Entity*>& p_entities )
{
	NetworkSynced* netSync = NULL;
	Transform* transform = NULL;

	for( unsigned int i=0; i<p_entities.size(); i++ )
	{
		netSync = static_cast<NetworkSynced*>(
			m_world->getComponentManager()->getComponent(
			p_entities[i]->getIndex(), ComponentType::NetworkSynced ) );

		if( netSync->getNetworkType() == NetworkType::Ship ||
			netSync->getNetworkType() == NetworkType::Prop)
		{


			transform = static_cast<Transform*>(
				m_world->getComponentManager()->getComponent(
				p_entities[i]->getIndex(), ComponentType::Transform ) );

			Packet updateEntityPacket;
			updateEntityPacket << (char)PacketType::EntityUpdate 
				<< (char)netSync->getNetworkType() 
				<< netSync->getNetworkIdentity() 
				<< transform->getTranslation() 
				<< transform->getRotation() 
				<< transform->getScale();

			m_server->broadcastPacket( updateEntityPacket );
		}
	}
}

void NetworkUpdateSystem::initialize()
{
}
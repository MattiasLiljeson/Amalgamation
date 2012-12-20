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

		if( netSync->getNetworkType() == NetworkType::Ship )
		{
			transform = static_cast<Transform*>(
				m_world->getComponentManager()->getComponent(
				p_entities[i]->getIndex(), ComponentType::Transform ) );

			// Useful debug movement.
			//static float timeElapsed = 0;
			//timeElapsed += m_world->getDelta();
			//AglVector3 pos = transform->getTranslation();
			//pos.z = 5.0f * sinf( timeElapsed ) + 6.0f;;
			//transform->setTranslation( pos );

			Packet updateShipPacket;
			updateShipPacket << (char)PacketType::EntityUpdate <<
				(char)NetworkType::Ship << netSync->getNetworkIdentity() <<
				transform->getTranslation() << transform->getRotation() <<
				transform->getScale();

			m_server->broadcastPacket( updateShipPacket );
		}
	}
}

void NetworkUpdateSystem::initialize()
{
}
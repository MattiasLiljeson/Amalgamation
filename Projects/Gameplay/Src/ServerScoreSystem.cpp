#include "ServerScoreSystem.h"
#include "PlayerComponent.h"
#include "NetworkSynced.h"
#include "PacketType.h"
#include "UpdateClientStatsPacket.h"

#include <TcpServer.h>
#include <Packet.h>


ServerScoreSystem::ServerScoreSystem( TcpServer* p_server )
	: EntitySystem( SystemType::NetworkUpdateScoresSystem,
	2, ComponentType::NetworkSynced, ComponentType::PlayerComponent )
{
	m_server = p_server;

	// Update interval time:
	m_updateTimer = m_updateTimerStartValue = 1.0f;
}

ServerScoreSystem::~ServerScoreSystem()
{
}

void ServerScoreSystem::processEntities( const vector<Entity*>& p_entities )
{
	// This system serves no function other than debugging, at the moment!!!
	m_updateTimer -= m_world->getDelta();

	if( m_updateTimer <= 0 )
	{
		m_updateTimer = m_updateTimerStartValue;

		for( unsigned int i=0; i<p_entities.size(); i++ )
		{
			NetworkSynced* netSync = static_cast<NetworkSynced*>(
				p_entities[i]->getComponent(ComponentType::NetworkSynced));

			PlayerComponent* score = static_cast<PlayerComponent*>(
				p_entities[i]->getComponent(ComponentType::PlayerComponent));
			//score->addBaseScore( 1 );
		}
	}
}

void ServerScoreSystem::initialize()
{

}
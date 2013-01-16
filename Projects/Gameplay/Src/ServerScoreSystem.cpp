#include "ServerScoreSystem.h"
#include "PlayerScore.h"
#include "NetworkSynced.h"
#include "PacketType.h"
#include "UpdateClientStatsPacket.h"

#include <TcpServer.h>
#include <Packet.h>


ServerScoreSystem::ServerScoreSystem( TcpServer* p_server )
	: EntitySystem( SystemType::NetworkUpdateScoresSystem,
	2, ComponentType::NetworkSynced, ComponentType::PlayerScore )
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
	m_updateTimer -= m_world->getDelta();

	if( m_updateTimer <= 0 )
	{
		m_updateTimer = m_updateTimerStartValue;

		for( unsigned int i=0; i<p_entities.size(); i++ )
		{
			NetworkSynced* netSync = static_cast<NetworkSynced*>(
				p_entities[i]->getComponent(ComponentType::NetworkSynced));

			PlayerScore* score = static_cast<PlayerScore*>(
				p_entities[i]->getComponent(ComponentType::PlayerScore));
			score->incrementScore( 1 );

			UpdateClientStatsPacket packet;
			packet.scores[i]		= score->getScore();
			packet.networkIds[i]	= netSync->getNetworkIdentity();

			m_server->broadcastPacket( packet.pack() );
		}
	}
}

void ServerScoreSystem::initialize()
{

}
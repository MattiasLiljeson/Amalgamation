#include "ServerWelcomeSystem.h"
#include <iostream>
#include <queue>
#include <vector>

#include <SystemType.h>
#include <ProcessMessageTerminate.h>
#include <ThreadSafeMessaging.h>
#include <TcpListenerProcess.h>
#include <DebugUtil.h>
#include <ToString.h>
#include <boost/thread/thread.hpp>

#include "PacketType.h"
#include "EntityType.h"

#include "ServerStaticObjectsSystem.h"
#include "ServerClientInfoSystem.h"

// Components:
#include "Transform.h"
#include "NetworkSynced.h"
#include "ShipFlyController.h"
#include "BodyInitData.h"
#include "PhysicsBody.h"
#include "PlayerComponent.h"
#include "ConnectionPointSet.h"
#include "GameplayTags.h"
#include "StaticProp.h"
#include "ClientInfo.h"
#include "LookAtEntity.h"
#include "LoadMesh.h"
#include "ShipConnectionPointHighlights.h"

// Packets
#include "EntityCreationPacket.h"
#include "WelcomePacket.h"
#include "NewlyConnectedPlayerPacket.h"
#include "DisconnectPacket.h"
#include "Packet.h"

#include <Globals.h>
#include "EntityFactory.h"
#include "PlayerSystem.h"



ServerWelcomeSystem::ServerWelcomeSystem(TcpServer* p_server, 
										 int p_activePort, int p_gameTime)
	: EntitySystem( SystemType::ServerWelcomeSystem, 1, ComponentType::NetworkSynced)
{
	m_server = p_server;
	m_gameTime = p_gameTime;
	m_activePort = p_activePort;
}

ServerWelcomeSystem::~ServerWelcomeSystem()
{
	m_server->stopListening();
}

void ServerWelcomeSystem::processEntities( const vector<Entity*>& p_entities )
{
	/************************************************************************/
	/* It goes here if there are new clients that has connected to the		*/
	/* server.																*/
	/************************************************************************/
	if ( m_server->isListening() )
	{
		while( m_server->hasNewConnections() )
		{
			int id = m_server->popNewConnection();

			// Creates a new client info entity for the newly connected client
			// createClientInfoEntity(id);

			/************************************************************************/
			/* Send the newly connected client a welcome packet with all inclusive. */
			/************************************************************************/
			sendWelcomePacket(id);
		}
	}
}

void ServerWelcomeSystem::initialize()
{
	m_server->startListening(m_activePort);
	m_playerSystem = static_cast<PlayerSystem*>
		(m_world->getSystem(SystemType::PlayerSystem));
}

void ServerWelcomeSystem::sendWelcomePacket(int p_newlyConnectedClientId)
{

	// Give the new client its Network Identity.
	WelcomePacket welcomePacket;
	welcomePacket.clientNetworkIdentity = p_newlyConnectedClientId;
	welcomePacket.serverGameTime = m_gameTime;
	welcomePacket.serverName = m_server->getServerName();
	welcomePacket.playerID = m_playerSystem->createPlayerId(p_newlyConnectedClientId);
	m_server->unicastPacket( welcomePacket.pack(), p_newlyConnectedClientId );
}

//void ServerWelcomeSystem::createClientInfoEntity( int p_newlyConnectedClientId )
//{
//	Entity* e = m_world->createEntity();
//	e->addComponent(ComponentType::ClientInfo, new ClientInfo(p_newlyConnectedClientId));
//	m_world->addEntity(e);
//}

//void ServerWelcomeSystem::sendBrodcastAllPlayers()
//{
//	vector<PlayerComponent*> playerComps = m_playerSystem->getPlayerComponents();
//	for(unsigned int i = 0; i < playerComps.size(); i++){
//		NewlyConnectedPlayerPacket connectedPacket;
//		connectedPacket.playerID = playerComps.at(i)->m_playerID;
//		connectedPacket.playerName = playerComps.at(i)->m_playerName;
//		m_server->broadcastPacket( connectedPacket.pack() );
//	}
//}

//void ServerWelcomeSystem::sendDisconnectPacket( int p_newlyDisconnectClientId )
//{
//	DisconnectPacket dcPacket;
//	dcPacket.clientNetworkIdentity = p_newlyDisconnectClientId;
////	dcPacket.playerID = m_playerSystem->get
//}

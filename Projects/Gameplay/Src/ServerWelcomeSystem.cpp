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
#include "PlayerScore.h"
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

#include <Globals.h>
#include "EntityFactory.h"



ServerWelcomeSystem::ServerWelcomeSystem( TcpServer* p_server, 
										 int p_activePort/* =1337 */ )
	: EntitySystem( SystemType::ServerWelcomeSystem, 1, ComponentType::NetworkSynced)
{
	m_server = p_server;
	m_activePort = p_activePort;
}

ServerWelcomeSystem::~ServerWelcomeSystem()
{
	m_server->stopListening();
}

void ServerWelcomeSystem::processEntities( const vector<Entity*>& p_entities )
{
	/************************************************************************/
	/* NOT USED RIGHT NOW! CLIENTS CANT DISCONNECT.							*/
	/************************************************************************/
	while (m_server->hasNewDisconnections())
	{
		int id = m_server->popNewDisconnection();

		for (unsigned int index = 0; index < p_entities.size(); index++)
		{
			NetworkSynced* netSync = static_cast<NetworkSynced*>(
				m_world->getComponentManager()->getComponent( p_entities[index],
					ComponentType::NetworkSynced ) );
		}

		// If a client has disconnected, then the clientinfo should be removed?
		auto clientInfoSys = static_cast<ServerClientInfoSystem*>(
			m_world->getSystem(SystemType::ServerClientInfoSystem));
		vector<Entity*> clientInfoEntities = clientInfoSys->getActiveEntities();
		for (unsigned int i = 0; i < clientInfoEntities.size(); i++)
		{
			auto clientInfo = static_cast<ClientInfo*>(
				clientInfoEntities[i]->getComponent(ComponentType::ClientInfo));

			if (clientInfo->id == id)
				m_world->deleteEntity(clientInfoEntities[i]);
		}
	}

	/************************************************************************/
	/* It goes here if there are new clients that has connected to the		*/
	/* server.																*/
	/************************************************************************/
	if ( m_server->isListening() && m_connectedPlayers.size() < 7)
	{
		while( m_server->hasNewConnections() )
		{
			int id = m_server->popNewConnection();

			// Creates a new client info entity for the newly connected client
			createClientInfoEntity(id);

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
}

void ServerWelcomeSystem::sendWelcomePacket(int p_newlyConnectedClientId)
{
	// Give the new client its Network Identity.
	WelcomePacket welcomePacket;
	welcomePacket.clientNetworkIdentity = p_newlyConnectedClientId;
	welcomePacket.playerID = m_connectedPlayers.size();
	m_server->unicastPacket( welcomePacket.pack(), p_newlyConnectedClientId );
}

void ServerWelcomeSystem::createClientInfoEntity( int p_newlyConnectedClientId )
{
	Entity* e = m_world->createEntity();
	e->addComponent(ComponentType::ClientInfo, new ClientInfo(p_newlyConnectedClientId));
	m_world->addEntity(e);
}

void ServerWelcomeSystem::addPlayer(int p_playerId, const string& p_playerName)
{
	m_connectedPlayers.push_back( PlayerInfo(p_playerName, p_playerId) );
}

void ServerWelcomeSystem::sendBrodcastAllPlayers()
{
	for(unsigned int i = 0; i < m_connectedPlayers.size(); i++){
		NewlyConnectedPlayerPacket connectedPacket;
		connectedPacket.playerID = m_connectedPlayers[i].ID;
		connectedPacket.playerName = m_connectedPlayers[i].name;
		m_server->broadcastPacket( connectedPacket.pack() );
	}
}

unsigned int ServerWelcomeSystem::getTotalOfConnectedPlayers()
{
	return m_connectedPlayers.size();
}

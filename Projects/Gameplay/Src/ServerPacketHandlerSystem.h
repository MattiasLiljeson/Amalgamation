#pragma once

#define FORCE_VS_DBG_OUTPUT

#include <map>
#include <EntitySystem.h>
#include "ClientInfo.h"
#include <string>
#include <Globals.h>

class TcpServer;
class PhysicsSystem;
class ServerStateSystem;

using namespace std;

// =======================================================================================
//                                  ServerPacketHandlerSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # ServerPacketHandlerSystem
/// Detailed description.....
/// Created on: 20-12-2012 
///---------------------------------------------------------------------------------------

class ServerPacketHandlerSystem : public EntitySystem
{
public:
	ServerPacketHandlerSystem( TcpServer* p_server , int p_gameTime);
	~ServerPacketHandlerSystem();

	void initialize();

	void processEntities( const vector<Entity*>& p_entities );
	void createAndBroadCastShip( int p_clientIdentity, int p_playerID );

	void printPacketTypeNotHandled(string p_state, int p_packetType);

private:
	Entity* createTheShipEntity(int p_newlyConnectedClientId, int p_playerID);
	void handleIngame();
	void handleLobby();
	void handleLoading();
	void handleSentAllPackets();
	void handleResult();

	void handleClientDisconnect();
	void handlePlayerDisconnect(int p_player);

	float stackBooster(Entity* p_parent);
private:
	ServerStateSystem* m_stateSystem;
	TcpServer*		m_server;
	PhysicsSystem*	m_physics;
	map<int,ClientInfo> m_clients; ///< The key is the clientID
	int m_finishedLoadingPlayers;
	int m_readyLoadingPlayers;
	int m_resultsPlayers; //Stupid name for a stupid variable
	int m_readyLobbyPlayers;
	bool m_lobbyPlayerReadyStates[MAXPLAYERS];
	int m_gameTime;
};
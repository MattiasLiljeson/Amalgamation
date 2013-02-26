#pragma once
#define WIN32_LEAN_AND_MEAN
#include <TcpServer.h>
#include <ModelBaseFactory.h>
#include <AglVector3.h>
#include <EntitySystem.h>
#include <string>

using namespace std;

class Transform;
class TcpServer;

// =======================================================================================
//                                      ServerWelcomeSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Using the TcpListenerProcess to listen for new connections.
///        
/// # NetworkListenerSystem
/// NetworkListenerSystem inherits the ThreadSafeMessaging which enables it to
/// communication thread safe with other processes.
/// It handles new incoming connections.
/// Created on: 3-12-2012 
///---------------------------------------------------------------------------------------


class ServerWelcomeSystem: public EntitySystem
{
public:
	ServerWelcomeSystem( TcpServer* p_server, int p_activePort=1337 );
	~ServerWelcomeSystem();

	virtual void processEntities(const vector<Entity*>& p_entities );

	virtual void initialize();
	unsigned int getTotalOfConnectedPlayers();

	void addPlayer(int p_playerId, const string& p_playerName);
	void sendBrodcastAllPlayers();
private:
	void sendWelcomePacket(int p_newlyConnectedClientId);
	void createClientInfoEntity(int p_newlyConnectedClientId);
private:
	struct PlayerInfo
	{
		int score;
		int ping;
		int ID;
		string name;

		PlayerInfo(string p_name, int p_ID){
			name = p_name;
			score = 0;
			ping = 0;
			ID = p_ID;
		}
	};
private:
	vector<PlayerInfo> m_connectedPlayers;
	TcpServer* m_server;
	int		m_activePort;
	ModelBaseFactory m_modelFactory;
};
#pragma once
#define WIN32_LEAN_AND_MEAN
#include <TcpServer.h>
#include <AglVector3.h>
#include <EntitySystem.h>
#include <string>

using namespace std;

class Transform;
class TcpServer;
class PlayerSystem;

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
	void sendBrodcastAllPlayers();
private:
	void sendWelcomePacket(int p_newlyConnectedClientId);
	void createClientInfoEntity(int p_newlyConnectedClientId);
private:
	TcpServer* m_server;
	PlayerSystem* m_playerSystem;
	int		m_activePort;

};
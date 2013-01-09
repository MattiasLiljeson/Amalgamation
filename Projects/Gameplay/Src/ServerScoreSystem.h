#pragma once

#include <EntitySystem.h>

class TcpServer;

// =======================================================================================
//	ServerScoreSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief A system that broadcasts the player's score.
///        
/// # NetworkUpdateScoresSystem
/// Broadcasts every connected players' score to every connected player, with a timer
/// that allows the sending to take place at given intervals.
/// Created on: 4-1-2013 
///---------------------------------------------------------------------------------------

class ServerScoreSystem: public EntitySystem
{
public:
	ServerScoreSystem( TcpServer* p_server );

	~ServerScoreSystem();

	void processEntities( const vector<Entity*>& p_entities );

	void initialize();

private:
	TcpServer* m_server;

	float m_updateTimer;
	float m_updateTimerStartValue;

};
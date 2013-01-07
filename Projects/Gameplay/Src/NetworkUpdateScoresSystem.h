#pragma once

#include <EntitySystem.h>

class TcpServer;

// =======================================================================================
//	NetworkUpdateScoresSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief A system that broadcasts the player's score.
///        
/// # NetworkUpdateScoresSystem
/// Broadcasts every connected players' score to every connected player, with a timer
/// that allows the sending to take place at given intervals.
/// Created on: 4-1-2013 
///---------------------------------------------------------------------------------------

class NetworkUpdateScoresSystem: public EntitySystem
{
public:
	NetworkUpdateScoresSystem( TcpServer* p_server );

	~NetworkUpdateScoresSystem();

	void processEntities( const vector<Entity*>& p_entities );

	void initialize();

private:
	TcpServer* m_server;

	float m_updateTimer;
	float m_updateTimerStartValue;

};
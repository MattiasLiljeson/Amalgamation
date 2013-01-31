#pragma once
#include <EntitySystem.h>
class TcpServer;
// =======================================================================================
//	WinningConditionSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief description...
///        
/// # WinningConditionSystem
/// Detailed description...
/// Created on: 30-1-2013 
///---------------------------------------------------------------------------------------

class WinningConditionSystem: public EntitySystem
{
public:
	WinningConditionSystem(TcpServer* p_server);
	~WinningConditionSystem();
	void process();
	///-----------------------------------------------------------------------------------
	/// Enables this system and starts a timer.
	/// \param p_endTime Decides how long the session is, stated in seconds.
	/// \return void
	///-----------------------------------------------------------------------------------
	void startGameSession(float p_endTime);

private:
	vector< pair<float, Entity*> > createSortedScoreEntityMapping();
	///-----------------------------------------------------------------------------------
	/// Sends a packet stating if a client won or lost. Currently only handles one winner.
	/// \param pair<float
	/// \param p_scoreComponentMapping
	/// \return void
	///-----------------------------------------------------------------------------------
	void signalEndSession(vector< pair<float, Entity*> > p_scoreComponentMapping);

private:
	float m_endTime;
	float m_elapsedGameSessionTime;
	TcpServer* m_server;

};
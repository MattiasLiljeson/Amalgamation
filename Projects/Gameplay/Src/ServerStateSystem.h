#pragma once
#include <EntitySystem.h>
#include "EnumGameStates.h"
// =======================================================================================
//                                      ServerStateSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Handles state changes
///        
/// # ServerStateSystem
/// Detailed description.....
/// Created on: 21-2-2013 
///---------------------------------------------------------------------------------------

class ServerStateSystem : public EntitySystem
{
public:
	struct State
	{
		ServerStates	stateEnum;
		EnumGameDelta	delta;

		State(){
			stateEnum = ServerStates::NONE;
			delta = EnumGameDelta::NOTCHANGED;
		}
		State(ServerStates p_state){
			stateEnum = p_state;
			delta	= EnumGameDelta::NOTCHANGED;
		}
	};
public:
	ServerStateSystem(ServerStates p_firstState);
	virtual ~ServerStateSystem();

	void setQueuedState( ServerStates p_state );
	EnumGameDelta getStateDelta( ServerStates p_state );
	ServerStates getCurrentState();

	virtual void process() final;

private:
	vector<State*> m_states;
	State m_queued;
	int m_currentState;
};
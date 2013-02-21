#pragma once
#include <Component.h>
#include "EnumGameStates.h"
// =======================================================================================
//                                      ServerGameState
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	The current state on the server side.
///        
/// # ServerGameState
/// Detailed description.....
/// Created on: 21-2-2013 
///---------------------------------------------------------------------------------------

class ServerGameState: public Component
{
public:
	struct State
	{
		ServerStates	stateEnum;
		EnumGameDelta	delta;

		State(ServerStates p_state){
			stateEnum = p_state;
			delta	= EnumGameDelta::NOTCHANGED;
		}
	};

	ServerGameState(ServerStates p_currentState)
		: Component(ComponentType::ServerGameState)
	{
		for(unsigned int i = 0 ; i < (unsigned int)ServerStates::NUMSTATES; i++){
			m_states.push_back(new State( static_cast<ServerStates>(i)));
		}
		setStatesDelta(p_currentState, EnumGameDelta::ENTEREDTHISFRAME);
	}

	~ServerGameState()
	{
		for (unsigned int i = 0 ; i < (unsigned int)ServerStates::NUMSTATES; i++){
			delete m_states[i];
		}
		m_states.clear();
	}

	void setStatesDelta( ServerStates p_state, EnumGameDelta p_value ){
		m_states[(int)p_state]->delta = p_value;
	}

	EnumGameDelta getStateDelta( ServerStates p_state ){
		return m_states[(int)p_state]->delta;
	}

public:
	vector<State*> m_states;

};
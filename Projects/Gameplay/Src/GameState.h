#pragma once
#include <Component.h>
#include "EnumGameStates.h"
// =======================================================================================
//                                      GameState
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Contains the current state the game is in right now
///        
/// # GameState
/// Detailed description.....
/// Created on: 13-2-2013 
///---------------------------------------------------------------------------------------

class GameState : public Component
{
public:
	struct State
	{
		EnumGameStates	stateEnum;
		int				delta;

		State(EnumGameStates p_state){
			stateEnum = p_state;
			delta  = 0;
		}
	};
public:
	GameState(EnumGameStates p_currentState);
	~GameState();
	void setStatesDelta(EnumGameStates p_state, int p_value);
	int getStateDelta(EnumGameStates p_state);
public:
	vector<State*> m_states;
};
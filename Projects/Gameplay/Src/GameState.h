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
		EnumGameDelta	delta;

		State(EnumGameStates p_state){
			stateEnum = p_state;
			delta	= NOTCHANGED;
		}
	};
public:
	GameState(EnumGameStates p_currentState);
	~GameState();
	void setStatesDelta(EnumGameStates p_state, EnumGameDelta p_value);
	int getStateDelta(EnumGameStates p_state);
public:
	vector<State*> m_states;
};
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
		GameStates	stateEnum;
		EnumGameDelta	delta;

		State(GameStates p_state){
			stateEnum = p_state;
			delta	= EnumGameDelta::NOTCHANGED;
		}
	};
public:
	GameState(GameStates p_currentState);
	~GameState();
	void setStatesDelta(GameStates p_state, EnumGameDelta p_value);
	EnumGameDelta getStateDelta(GameStates p_state);
public:
	vector<State*> m_states;
};
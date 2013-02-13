#include "GameState.h"


GameState::GameState( EnumGameStates p_currentState ) : Component(ComponentType::GameState)
{
	for(unsigned int i = 0 ; i < EnumGameStates::NUMSTATES; i++){
		m_states.push_back(new State( static_cast<EnumGameStates>(i)));
	}
	setStatesDelta(p_currentState,1);
}

GameState::~GameState()
{
	for (unsigned int i = 0 ; i < EnumGameStates::NUMSTATES; i++){
		delete m_states[i];
	}
	m_states.clear();
}

void GameState::setStatesDelta( EnumGameStates p_state, int p_value ){
	m_states[p_state]->delta = p_value;
}
#include "GameState.h"


GameState::GameState( GameStates p_currentState ) : Component(ComponentType::GameState)
{
	for(unsigned int i = 0 ; i < (unsigned int)(GameStates::NUMSTATES); i++){
		m_states.push_back(new State( static_cast<GameStates>(i)));
	}
	setStatesDelta(p_currentState,EnumGameDelta::ENTEREDTHISFRAME);
}

GameState::~GameState()
{
	for (unsigned int i = 0 ; i < (unsigned int)(GameStates::NUMSTATES); i++){
		delete m_states[i];
	}
	m_states.clear();
}

void GameState::setStatesDelta( GameStates p_state, EnumGameDelta p_value ){
	m_states[(int)p_state]->delta = p_value;
}

EnumGameDelta GameState::getStateDelta( GameStates p_state ){
	return m_states[(int)p_state]->delta;
}

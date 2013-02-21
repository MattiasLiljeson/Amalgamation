#include "ClientStateSystem.h"

ClientStateSystem::ClientStateSystem( GameStates p_firstState )
	: EntitySystem(SystemType::ClientStateSystem)
{
	for(unsigned int i = 0 ; i < (unsigned int)GameStates::NUMSTATES; i++){
		m_states.push_back(new State( static_cast<GameStates>(i)));
	}
	m_currentState = (int)GameStates::INIT;
	setQueuedState(p_firstState);
}

ClientStateSystem::~ClientStateSystem()
{
	for (unsigned int i = 0 ; i < (unsigned int)GameStates::NUMSTATES; i++){
		delete m_states[i];
	}
	m_states.clear();
}

void ClientStateSystem::process()
{
	//A state change has been requested and handled
	if(m_queued.stateEnum != m_states[m_currentState]->stateEnum){
		m_states[m_currentState]->delta = EnumGameDelta::EXITTHISFRAME;
		m_currentState = (int)m_queued.stateEnum;
		m_states[m_currentState]->delta = EnumGameDelta::ENTEREDTHISFRAME;
	}
	else{
		for (unsigned int i = 0; i < (unsigned int)GameStates::NUMSTATES; i++){
			m_states[i]->delta = EnumGameDelta::NOTCHANGED;
		}
	}
}

void ClientStateSystem::setQueuedState( GameStates p_state )
{
	m_queued.stateEnum = p_state;
}

EnumGameDelta ClientStateSystem::getStateDelta( GameStates p_state )
{
	return m_states[(int)p_state]->delta;
}

GameStates ClientStateSystem::getCurrentState()
{
	return m_states[m_currentState]->stateEnum;
}
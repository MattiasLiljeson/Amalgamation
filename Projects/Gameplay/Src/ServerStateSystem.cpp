#include "ServerStateSystem.h"
#define FORCE_VS_DBG_OUTPUT
#include <DebugUtil.h>

ServerStateSystem::ServerStateSystem(ServerStates p_firstState)
	: EntitySystem(SystemType::ServerStateSystem)
{
	for(unsigned int i = 0 ; i < (unsigned int)ServerStates::NUMSTATES; i++){
		m_states.push_back(new State( static_cast<ServerStates>(i)));
	}
	m_currentState = (int)ServerStates::NONE;
	setQueuedState(p_firstState);
}

ServerStateSystem::~ServerStateSystem()
{
	for (unsigned int i = 0 ; i < (unsigned int)ServerStates::NUMSTATES; i++){
		delete m_states[i];
	}
	m_states.clear();
}

void ServerStateSystem::process()
{
	//A state change has been requested and handled
	if(m_queued.stateEnum != m_states[m_currentState]->stateEnum){
		m_states[m_currentState]->delta = EnumGameDelta::EXITTHISFRAME;
		m_currentState = (int)m_queued.stateEnum;
		m_states[m_currentState]->delta = EnumGameDelta::ENTEREDTHISFRAME;

		switch (m_states[m_currentState]->stateEnum)
		{
		case ServerStates::INGAME:
			DEBUGPRINT(("SERVER: Changed to ingame state!\n"));
			break;
		case ServerStates::LOADING:
			DEBUGPRINT(("SERVER: Changed to loading state!\n"));
			break;
		case ServerStates::LOBBY:
			DEBUGPRINT(("SERVER: Changed to lobby state!\n"));
			break;
		case ServerStates::RESULT:
			DEBUGPRINT(("SERVER: Changed to result state!\n"));
			break;
		case ServerStates::SENTALLPACKETS:
			DEBUGPRINT(("SERVER: Changed to sent all packet state!\n"));
			break;
		default:
			break;
		}
	}
	else{
		for (unsigned int i = 0; i < (unsigned int)ServerStates::NUMSTATES; i++){
			m_states[i]->delta = EnumGameDelta::NOTCHANGED;
		}
	}
}

void ServerStateSystem::setQueuedState( ServerStates p_state){
	m_queued.stateEnum = p_state;
}

EnumGameDelta ServerStateSystem::getStateDelta( ServerStates p_state ){
	return m_states[(int)p_state]->delta;
}

ServerStates ServerStateSystem::getCurrentState()
{
	return m_states[m_currentState]->stateEnum;
}
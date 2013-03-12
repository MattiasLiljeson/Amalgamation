#include "ClientStateSystem.h"
#define FORCE_VS_DBG_OUTPUT
#include <DebugUtil.h>


ClientStateSystem::ClientStateSystem( GameStates p_firstState )
	: EntitySystem(SystemType::ClientStateSystem)
{
	for(unsigned int i = 0 ; i < (unsigned int)GameStates::NUMSTATES; i++){
		m_states.push_back(new State( static_cast<GameStates>(i)));
	}
	m_currentState = (int)GameStates::NONE;
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

		switch (m_states[m_currentState]->stateEnum)
		{
		case GameStates::INGAME:
			DEBUGPRINT(("CLIENT: Changed to ingame state!\n"));
			break;
		case GameStates::MENU:
			DEBUGPRINT(("CLIENT: Changed to menu state!\n"));
			break;
		case GameStates::INITGAME:
			DEBUGPRINT(("CLIENT: Changed to init game state!\n"));
			break;
		case GameStates::LOBBY:
			DEBUGPRINT(("CLIENT: Changed to lobby state!\n"));
			break;
		case GameStates::LOADING:
			DEBUGPRINT(("CLIENT: Changed to loading state!\n"));
			break;
		case GameStates::FINISHEDLOADING:
			DEBUGPRINT(("CLIENT: Changed to finished loading state!\n"));
			break;
		case GameStates::PAUSE:
			DEBUGPRINT(("CLIENT: Changed to pause state!\n"));
			break;
		case GameStates::RESULTS:
			DEBUGPRINT(("CLIENT: Changed to results!\n"));
			break;
		default:
			DEBUGPRINT(("CLIENT: Changed to a unkown state!\n"));
			break;
		}
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
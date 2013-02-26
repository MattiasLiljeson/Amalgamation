#pragma once
#include <EntitySystem.h>
#include "EnumGameStates.h"
// =======================================================================================
//                                      ClientStateSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Handles the game states on client side.
///        
/// # ClientStateSystem
/// Detailed description.....
/// Created on: 21-2-2013 
///---------------------------------------------------------------------------------------

class ClientStateSystem: public EntitySystem
{
public:
	struct State
	{
		GameStates stateEnum;
		EnumGameDelta delta;

		State(){
			stateEnum = GameStates::NONE;
			delta = EnumGameDelta::NOTCHANGED;
		}
		State(GameStates p_state){
			stateEnum = p_state;
			delta = EnumGameDelta::NOTCHANGED;
		}
	};

public:
	ClientStateSystem(GameStates p_firstState);
	~ClientStateSystem();
	void setQueuedState( GameStates p_state );
	EnumGameDelta getStateDelta( GameStates p_state );
	GameStates getCurrentState();

	virtual void process() final;

private:
	vector<State*> m_states;
	State m_queued;
	int m_currentState;

};
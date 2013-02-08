#pragma once
#include <Component.h>
#include <ComponentFactory.h>
#include <PlayerStates.h>
// =======================================================================================
//                                      PlayerState
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	State
///        
/// # PlayerState
/// Detailed description.....
/// Created on: 5-2-2013 
///---------------------------------------------------------------------------------------

class PlayerState : public Component
{
public:

	PlayerState() : Component(ComponentType::PlayerState)
	{state=PlayerStates::steeringState;}
	virtual ~PlayerState() {}

	int state;
};
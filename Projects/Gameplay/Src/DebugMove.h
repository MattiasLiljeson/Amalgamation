#pragma once

#include <Component.h>
#include <AglVector3.h>

// =======================================================================================
//	DebugMove
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief description...
///        
/// # DebugMove
/// Detailed description...
/// Created on: 23-1-2013 
///---------------------------------------------------------------------------------------

class DebugMove: public Component
{
public:
	DebugMove(AglVector3 p_direction)
	: Component( ComponentType::DebugMove )
	{
		direction = p_direction;
	}
	
public:
	// Direction and length of the debug movement
	AglVector3 direction;

};
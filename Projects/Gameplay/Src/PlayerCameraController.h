#pragma once

// =======================================================================================
//                                   PlayerCameraController
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Player camera controller component. Defines controller properties of 
/// the camera during gameplay.
///        
/// # PlayerCameraController
/// Detailed description.....
/// Created on: 13-12-2012 
///---------------------------------------------------------------------------------------

#include <Component.h>
#include <DebugUtil.h>

class PlayerCameraController : public Component
{
public:
	PlayerCameraController(float p_orbitspeed=1.0f)
	{
		orbitSpeed=p_orbitspeed;
	}

	~PlayerCameraController() {}
	AglVector3 accumulatedCameraMovement;
	float orbitSpeed;
private:

};
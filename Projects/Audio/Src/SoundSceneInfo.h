#pragma once
#include <AglVector3.h>

// =======================================================================================
//                                      SoundSceneInfo
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Contains the needed information for the sound
///        
/// # SoundSceneInfo
/// Detailed description.....
/// Created on: 7-12-2012 
///---------------------------------------------------------------------------------------

struct SoundSceneInfo
{
	AglVector3 listenerPos;
	AglVector3 listenerOrientFront;
	AglVector3 listenerOrientTop;
	AglVector3 listenerVelocity;
};
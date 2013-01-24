#pragma once
#include <x3daudio.h>
#include "SoundOrientation.h"

// =======================================================================================
//                                      PositionalSoundCreationInfo
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Contains information to properly create a sound
///        
/// # SoundCreationStruct
/// Detailed description.....
/// Created on: 13-12-2012 
///---------------------------------------------------------------------------------------

struct PositionalSoundCreationInfo
{
	X3DAUDIO_DSP_SETTINGS* settings;
	unsigned int destChannels;
	SoundOrientation soundOrientation;
	PositionalSoundCreationInfo(const AglVector3& p_pos)
	{
		soundOrientation.listenerPos = p_pos;
		soundOrientation.listenerOrientFront = AglVector3(0,0,1);
		soundOrientation.listenerOrientTop = AglVector3(0,1,0);
		soundOrientation.listenerVelocity = AglVector3(0,0,0);
		destChannels = 0;
		settings = NULL;
	}
};
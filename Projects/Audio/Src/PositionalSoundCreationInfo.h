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
	FLOAT32* outputMatrix;
	//FLOAT32* emitterAzimuths;

	X3DAUDIO_DSP_SETTINGS settings;

	bool	loopPlayback;
	SoundOrientation soundOrientation;
};
#pragma once
#include <xaudio2.h>
#include <x3daudio.h>
#include "SoundOrientation.h"

// =======================================================================================
//                                      SoundCreationStruct
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Contains information to properly create a sound
///        
/// # SoundCreationStruct
/// Detailed description.....
/// Created on: 13-12-2012 
///---------------------------------------------------------------------------------------

struct SoundCreationInfo
{
	FLOAT32* outputMatrix;
	FLOAT32* emitterAzimuths;

	int		destinationChannels;
	bool	loopPlayback;
	SoundOrientation soundOrientation;
};
#pragma once
#include <x3daudio.h>
#include <AglVector3.h>
#include "SoundOrientation.h"

// =======================================================================================
//                                      PositionalSoundInfo
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Contains the information needed to use a positional sound
///        
/// # PositionalSoundInfo
/// Detailed description.....
/// Created on: 11-12-2012 
///---------------------------------------------------------------------------------------

struct PositionalSoundInfo
{
	X3DAUDIO_EMITTER		emitter;
	X3DAUDIO_DSP_SETTINGS	settings;
	X3DAUDIO_CONE			cone;

	PositionalSoundInfo()
	{

	}
};
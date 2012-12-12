#pragma once
#include <x3daudio.h>

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
	float pos[3];

	X3DAUDIO_EMITTER emitter;

	PositionalSoundInfo(float x, float y, float z)
	{
		pos[0] = x;
		pos[1] = y;
		pos[2] = z;
	}
	PositionalSoundInfo()
	{

	}
};
#pragma once
#include <xaudio2.h>
#include <string>

// =======================================================================================
//                                      BasicSoundCreationInfo
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Contains all the needed information to create a basic sound
///        
/// # AmbientSoundCreationInfo
/// Detailed description.....
/// Created on: 14-12-2012 
///---------------------------------------------------------------------------------------

struct BasicSoundCreationInfo
{
	float volume;
	IXAudio2Voice* destinationVoice;
	const char* file;
	bool loopPlayback;

	BasicSoundCreationInfo(const char* p_filePath, bool p_loopPlayback=false, float p_volume=1.0f)
	{
		destinationVoice = NULL;
		volume = p_volume;
		file = p_filePath;
		loopPlayback = p_loopPlayback;
	}
	/*
	BasicSoundCreationInfo(std::string p_filePath, bool p_loopPlayback=false, 
		float p_volume=1.0f)
	{
		destinationVoice = NULL;
		volume = p_volume;
		loopPlayback = p_loopPlayback;
	}*/
};
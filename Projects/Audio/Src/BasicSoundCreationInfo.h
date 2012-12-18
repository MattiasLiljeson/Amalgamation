#pragma once
#include <xaudio2.h>

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
	const char* file; //ONLY USED FOR DEBUG!
	const char* fullFilePath;
	bool loopPlayback;

	BasicSoundCreationInfo(const char* p_file, const char* p_fullFilePath, 
		bool p_loopPlayback=false, float p_volume=1.0f)
	{
		destinationVoice = NULL;
		volume = p_volume;
		file = p_file;
		fullFilePath = p_fullFilePath;
		loopPlayback = p_loopPlayback;
	}
};
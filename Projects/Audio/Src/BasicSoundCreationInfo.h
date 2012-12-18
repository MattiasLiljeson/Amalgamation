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
	const char* file; //ONLY USED FOR DEBUG!
	const char* filePath;
	bool loopPlayback;

	BasicSoundCreationInfo(const char* p_file, const char* p_filePath, 
		bool p_loopPlayback=false, float p_volume=1.0f)
	{
		destinationVoice = NULL;
		volume = p_volume;
		file = p_file;
		filePath = p_filePath;
		loopPlayback = p_loopPlayback;
	}

	std::string getFullFilePathString()
	{
		std::string fullFilePath = filePath;
		fullFilePath += file;
		return fullFilePath;
	}
};
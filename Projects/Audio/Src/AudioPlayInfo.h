#pragma once

#include <AglVector3.h>
#include <string>
#include "AudioHeader.h"

// =======================================================================================
//                                      AudioPlayInfo
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Contains all the information used to create and use a sound
///        
/// # AudioPlayInfo
/// Detailed description.....
/// Created on: 12-3-2013 
///---------------------------------------------------------------------------------------

struct AudioPlayInfo{

public:

	/*float timerInterval; // Only used if the play interval is set to TIMERBASED Measured in s
	float volume;  
	float frequency; //Used for to pitch sounds standard 1.0f
	float maxRange; //Used for ambient range
	float minRange;	//Used to specify a minimum range when the volume is 1.0f
	float dopplerScalar;
	float dopplerFactor;
	AglVector3 pos;
	AglVector3 front;
	AglVector3 top;
	AglVector3 velocity;
	AudioHeader::PlayInterval playInterval;
	AudioHeader::PlayState playingState;*/
	AudioHeader::PlayState queuedPlayingState;
	AudioHeader::SoundType soundType;

public:
	AudioPlayInfo(const AudioHeader::SoundType& p_soundType, const std::string& p_soundName){
		queuedPlayingState = AudioHeader::STOP;
		soundType = p_soundType;
	}
};
#pragma once

#include <AglVector3.h>
#include <string>


// =======================================================================================
//                                      AudioHeader
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Contains all the information used to create and use a sound
///        
/// # AudioHeader
/// Detailed description.....
/// Created on: 5-3-2013 
///---------------------------------------------------------------------------------------

struct AudioHeader{
	enum PlayInterval{
		ONCE, TWICE, FOREVER
	};

	enum PlayState{
		PLAY, STOP, PAUSE, RESTART, RESUME
	};

	enum SoundType{
		AMBIENT, AMBIENTRANGE, POSITIONALSOUND, NUMSOUNDTYPES
	};
	float maxFrequencyOffeset; //Used for Doppler standard 1.0f
	float maxRange;
	float minRange;
	float dopplerScalar;
	float dopplerFactor;
	unsigned int sourceChannels;
	unsigned int soundIndex;
	AglVector3 pos;
	AglVector3 front;
	AglVector3 top;
	AglVector3 velocity;
	PlayInterval playInterval;
	PlayState playingState;
	PlayState queuedPlayingState;
	SoundType soundType;
	std::string path;
	std::string file;

	AudioHeader(const SoundType& p_soundType){
		maxRange = 1.0f;
		minRange = -1;
		maxFrequencyOffeset = 1.0f;
		dopplerScalar = 1.0f;
		dopplerFactor = 1.0f;
		sourceChannels = 1.0f;
		soundIndex = -1;
		pos = AglVector3::zero();
		front = AglVector3::forward();
		top = AglVector3::up();
		velocity = AglVector3::zero();
		playInterval = ONCE;
		playingState = STOP;
		queuedPlayingState = STOP;
		soundType = p_soundType;

	}
};
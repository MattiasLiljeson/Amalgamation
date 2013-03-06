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
		ONCE, TWICE, FOREVER, TIMERBASED
	};

	enum PlayState{
		PLAY, STOP, PAUSE, RESTART, RESUME
	};

	enum SoundType{
		AMBIENT, AMBIENTRANGE, POSITIONALSOUND, NUMSOUNDTYPES
	};

	float timerInterval; // Only used if the play interval is set to TIMERBASED Measured in s
	float timeSinceLastPlayed; // Only used if the play interval is set to TIMERBASED in s
	float maxFrequencyOffeset;
	float frequency; //Used for to pitch sounds standard 1.0f
	float maxRange; //Used for ambient range
	float minRange;	//Used to specify a minimum range when the volume is 1.0f
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
		timerInterval = 0;
		timeSinceLastPlayed = 0;
		maxRange = 1.0f;
		minRange = -1;
		frequency = 1.0f;
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
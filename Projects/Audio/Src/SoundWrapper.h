#pragma once

#include <XAudio2.h>
#include <XAudio2fx.h>
#include <X3daudio.h>

// =======================================================================================
//                                      SoundWrapper
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Wraps the sound functionality to become easier to use
///        
/// # SoundWrapper
/// Detailed description.....
/// Created on: 6-12-2012 
///---------------------------------------------------------------------------------------

class SoundWrapper
{
public:
	SoundWrapper();
	virtual ~SoundWrapper();
private:
	void initSoundEngine();
private:
	IXAudio2* m_soundeDevice;
	IXAudio2MasteringVoice* m_masterVoice;

	float m_masterVolume;
	DWORD m_destChannels;
};
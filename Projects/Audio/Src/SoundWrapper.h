#pragma once

#include "AudioEngineCreatorh.h"
#include "XAudio2Exception.h"
#include "SoundSceneInfo.h"
#include "AudioCurves.h"
#include "Sound.h"
#include "PositionalSound.h"
#include "SoundFactory.h"

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
	void updateListener(const SoundSceneInfo& p_sceneInfo);
	Sound* createNewNonPositionalSound(const char* p_filePath);
	PositionalSound* createNewPositionalSound(const char* p_filePath);
private:
	void initSoundEngine();
private:
	IXAudio2* m_soundDevice;
	IXAudio2MasteringVoice* m_masterVoice;

	float m_masterVolume;
	DWORD m_destChannels;

	XAUDIO2_DEVICE_DETAILS details;

	X3DAUDIO_LISTENER		m_listener;
	X3DAUDIO_HANDLE			m_x3DAudioInstance;
	X3DAUDIO_DSP_SETTINGS	m_dspSettings;

	SoundFactory*			m_soundFactory;
};
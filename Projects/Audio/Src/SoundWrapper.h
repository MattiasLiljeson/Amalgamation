#pragma once

#include <AglVector3.h>
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
/// Listener position will always be initialized to 
/// Position (0,0,0)
/// OrientTop (0,1,0)
/// OrientFront (0,0,1)
/// Velocity (0,0,0)
/// Created on: 6-12-2012 
///---------------------------------------------------------------------------------------

class SoundWrapper
{
public:
	SoundWrapper();
	virtual ~SoundWrapper();
	void updateListener(const SoundSceneInfo& p_sceneInfo);
	Sound* createNewNonPositionalSound(const char* p_filePath);
	PositionalSound* createNewPositionalSound(const char* p_filePath, AglVector3 p_pos);

	void update(PositionalSound* p_sound);
	void setListenerPos(AglVector3 p_newPos);
private:
	void initSoundEngine();
	void init3DSoundSettings();
	void initListener();
private:
	const static int SOURCECHANNEL = 1;
	IXAudio2* m_soundDevice;
	IXAudio2MasteringVoice* m_masterVoice;

	float m_masterVolume;
	UINT32 m_destChannels;

	XAUDIO2_DEVICE_DETAILS m_details;

	/************************************************************************/
	/* 3D Audio needed information											*/
	/************************************************************************/
	X3DAUDIO_LISTENER		m_listener;
	X3DAUDIO_HANDLE			m_x3DAudioInstance;
	X3DAUDIO_DSP_SETTINGS	m_dspSettings;

	FLOAT32*				m_matrixCoefficients;
	FLOAT32*				m_emitterAzimuths;

	SoundFactory*			m_soundFactory;
};
#include "SoundWrapper.h"

SoundWrapper::SoundWrapper()
{
	m_soundeDevice = NULL;
	m_masterVoice  = NULL;
	m_masterVolume = 0;
}

void SoundWrapper::initSoundEngine()
{
	HRESULT hr = S_OK;
	UINT32 flags = 0;
	XAUDIO2_DEBUG_CONFIGURATION debug_level;
	debug_level.TraceMask = XAUDIO2_LOG_ERRORS | XAUDIO2_LOG_WARNINGS;


#ifdef _DEBUG
//	IXAudio2::SetDebugConfiguration(debug_level);
#endif
	// Send in the wanted flags when the sound device is created.
	if( FAILED( hr = XAudio2Create(&m_soundDevice, flags)) )
		throw 

	if( FAILED( hr = m_soundDevice->CreateMasteringVoice(&m_masterVoice)))
	{
		return hr;
	}

	XAUDIO2_DEVICE_DETAILS details;
	if( FAILED( hr = m_soundDevice->GetDeviceDetails(0, &details)))
	{
		return hr;
	}

	m_destChannels = details.OutputFormat.Format.nChannels;

	X3DAudioInitialize( details.OutputFormat.dwChannelMask, X3DAUDIO_SPEED_OF_SOUND, 
		m_X3DAudioInstance);

	m_matrixCoefficients	= new FLOAT32[1*details.OutputFormat.Format.nChannels];
	m_emitterAzimuths		= new FLOAT32[1];

	initPositionalSound();

	return hr;
}

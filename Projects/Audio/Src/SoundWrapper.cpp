#include "SoundWrapper.h"

SoundWrapper::SoundWrapper()
{
	m_soundDevice	= NULL;
	m_masterVoice	= NULL;
	m_masterVolume	= 0;

	//required by XAudio2
	CoInitializeEx( NULL, COINIT_MULTITHREADED );

	initSoundEngine();
}

SoundWrapper::~SoundWrapper()
{
	m_masterVoice->DestroyVoice();
	m_soundDevice->StopEngine();
	m_soundDevice->Release();
}

void SoundWrapper::initSoundEngine()
{
	HRESULT hr = S_OK;
	UINT32 flags = 0;
#ifdef _DEBUG
	flags |= XAUDIO2_DEBUG_ENGINE;
#endif

	// Send in the wanted flags when the sound device is created.
	if( FAILED( hr = XAudio2Create(&m_soundDevice, flags)) )
		throw XAudio2Exception(hr,__FILE__,__FUNCTION__,__LINE__);

	if( FAILED( hr = m_soundDevice->CreateMasteringVoice(&m_masterVoice)))
		throw XAudio2Exception(hr,__FILE__,__FUNCTION__,__LINE__);

	/*
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

	return hr;*/
}


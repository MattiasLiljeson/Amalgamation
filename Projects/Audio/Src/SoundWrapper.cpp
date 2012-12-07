#include "SoundWrapper.h"

SoundWrapper::SoundWrapper()
{
	m_soundDevice	= NULL;
	m_masterVoice	= NULL;
	m_masterVolume	= 0;

	//required by XAudio2
	CoInitializeEx( NULL, COINIT_MULTITHREADED );

	initSoundEngine();

	m_soundFactory = new SoundFactory();
}

SoundWrapper::~SoundWrapper()
{
	m_masterVoice->DestroyVoice();
	m_soundDevice->StopEngine();
	m_soundDevice->Release();
	
	delete m_soundFactory;
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

	if( FAILED( hr = m_soundDevice->GetDeviceDetails(0, &details)))
		throw XAudio2Exception(hr,__FILE__,__FUNCTION__,__LINE__);

	m_destChannels = details.OutputFormat.Format.nChannels;

	X3DAudioInitialize( details.OutputFormat.dwChannelMask, X3DAUDIO_SPEED_OF_SOUND, 
		m_x3DAudioInstance);
}

void SoundWrapper::updateListener(const SoundSceneInfo& p_sceneInfo)
{
	X3DAUDIO_VECTOR front = {
		p_sceneInfo.listenerOrientFront[0],
		p_sceneInfo.listenerOrientFront[1],
		p_sceneInfo.listenerOrientFront[2],
	};

	X3DAUDIO_VECTOR top = {
		p_sceneInfo.listenerOrientFront[0],
		p_sceneInfo.listenerOrientFront[1],
		p_sceneInfo.listenerOrientFront[2],
	};
	
	X3DAUDIO_VECTOR velocity =  {
		p_sceneInfo.listenerVelocity[0],
		p_sceneInfo.listenerVelocity[1],
		p_sceneInfo.listenerVelocity[2],
	};

	X3DAUDIO_VECTOR pos = {
		p_sceneInfo.listenerPos[0],
		p_sceneInfo.listenerPos[1],
		p_sceneInfo.listenerPos[2],
	};

	m_listener.OrientFront	= front;
	m_listener.OrientTop	= top;
	m_listener.Position		= pos;
	m_listener.Velocity		= velocity;
	m_listener.pCone		= NULL;
}

Sound* SoundWrapper::createNewNonPositionalSound( const char* p_filePath )
{
	m_soundFactory->
}


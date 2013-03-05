#include "SoundWrapper.h"


SoundWrapper::SoundWrapper()
{
	m_soundDevice	= NULL;
	m_masterVoice	= NULL;
	m_channelVolumes = NULL;
	m_masterVolume	= 1.0f;	// NOTE: (Johan) Makes no difference if this is changed.
	
	ZeroMemory(&m_details, sizeof(XAUDIO2_DEVICE_DETAILS));

	/************************************************************************/
	/* XAudio2 2.7 specific call, this is called behind the scene in later	*/
	/* versions	of XAudio2.													*/
	/************************************************************************/
	CoInitializeEx( NULL, COINIT_MULTITHREADED );

	initSoundEngine();

	m_soundFactory = new SoundFactory(m_soundDevice);
}

SoundWrapper::~SoundWrapper()
{

	for (unsigned int i = 0; i < m_sounds.getSize(); i++)
	{
		destroySound(i);
	}
	
	for (unsigned int i = 0; i <m_activeSounds.size(); i++)
	{
		delete m_activeSounds[i];
	}
	m_activeSounds.clear();
	for(unsigned int i=0; i<m_soundEffects.size(); i++)
	{
		if( m_soundEffects[i] )
		{
			delete m_soundEffects[i];
		}
	}

	m_masterVoice->DestroyVoice();
	m_soundDevice->StopEngine();
	m_soundDevice->Release();
	
	delete m_soundFactory;
	delete m_channelVolumes;

	CoUninitialize();
}

void SoundWrapper::initSoundEngine()
{
	HRESULT hr = S_OK;

	UINT32 flags = 0;
#ifdef _DEBUG
	flags |= XAUDIO2_DEBUG_ENGINE;
#endif
	if ( FAILED (hr = XAudio2Create(&m_soundDevice,flags)))
		throw XAudio2Exception(hr,__FILE__,__FUNCTION__,__LINE__);

	if( FAILED( hr = m_soundDevice->CreateMasteringVoice(&m_masterVoice)))
		throw XAudio2Exception(hr,__FILE__,__FUNCTION__,__LINE__);

	/************************************************************************/
	/* XAudio 2.7 specific variable XAUDIO2_DEVICE_DETAILS					*/
	/* Get all the juicy details about the sound device.					*/
	/************************************************************************/
	if ( FAILED( hr = m_soundDevice->GetDeviceDetails( 0, &m_details ) ) )
		throw XAudio2Exception(hr,__FILE__,__FUNCTION__,__LINE__);

	m_destChannels	= m_details.OutputFormat.Format.nChannels;
	m_channelMask	= m_details.OutputFormat.dwChannelMask; 
	m_channelVolumes = new float[m_destChannels];
	init3DSoundSettings();
	initListener();
}

void SoundWrapper::updateListener(const SoundOrientation& p_sceneInfo)
{
	X3DAUDIO_VECTOR front = {
		p_sceneInfo.listenerOrientFront[0],
		p_sceneInfo.listenerOrientFront[1],
		p_sceneInfo.listenerOrientFront[2],
	};

	X3DAUDIO_VECTOR top = {
		p_sceneInfo.listenerOrientTop[0],
		p_sceneInfo.listenerOrientTop[1],
		p_sceneInfo.listenerOrientTop[2],
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
/*
int SoundWrapper::createAmbientSound(BasicSoundCreationInfo* p_info)
{
	
	int soundIndex = -1;
	Sound* sound = NULL;
	if(p_info->loopPlayback)
	{
		sound = m_soundFactory->createAmbientSound(p_info);
		m_activeSounds.push_back(sound);
		soundIndex = (int)m_activeSounds.size()-1;
	}
	else
	{
		sound = m_soundFactory->createAmbientSoundEffect(p_info,
			(int)m_soundEffects.size(), &m_soundEffects);
		m_soundEffects.push_back(sound);
		sound->resumeOrPlay();
	}
	return soundIndex;
}
*/

/*
int SoundWrapper::createNewPositionalSound(BasicSoundCreationInfo* p_basicSoundInfo, 
										   PositionalSoundCreationInfo* p_positionalInfo)
{
	int soundIndex = -1;
	p_positionalInfo->destChannels = m_destChannels;
	PositionalSound* positionalSound = NULL;
	if(p_basicSoundInfo->loopPlayback)
	{
		positionalSound = m_soundFactory->createPositionalSound(
			p_basicSoundInfo, p_positionalInfo);
		m_activeSounds.push_back(positionalSound);
		soundIndex = (int)m_activeSounds.size()-1;
	}
	else
	{
		positionalSound = m_soundFactory->createPositionalSoundEffect(
			p_basicSoundInfo, p_positionalInfo, (int)m_soundEffects.size(),
			&m_soundEffects);
		m_soundEffects.push_back(positionalSound);
		positionalSound->resumeOrPlay();
	}
	return soundIndex;
}
*/

void SoundWrapper::init3DSoundSettings()
{
	/************************************************************************/
	/* The number of output channels have to be know before the 3D			*/
	/* calculations can be used. The speed of sound can be altered for		*/
	/* artistic feeling.													*/
	/************************************************************************/
	X3DAudioInitialize( m_channelMask, X3DAUDIO_SPEED_OF_SOUND,	m_x3DAudioInstance);
}

void SoundWrapper::initListener()
{
	SoundOrientation info;
	info.listenerOrientFront	= AglVector3(1,0,0);
	info.listenerOrientTop		= AglVector3(0,1,0);
	info.listenerPos			= AglVector3(0,0,0);
	info.listenerVelocity		= AglVector3(0,0,0);

	updateListener(info);
}

void SoundWrapper::updateOutputMatrix(int p_index)
{
	PositionalSound* sound = static_cast<PositionalSound*>(m_activeSounds[p_index]);
	X3DAudioCalculate(m_x3DAudioInstance, &m_listener, &sound->getEmitter(), 
		X3DAUDIO_CALCULATE_MATRIX, &sound->getDSPSettings());

	//m_masterVoice->GetChannelVolumes(2,m_s)

	sound->m_left = sound->getDSPSettings().pMatrixCoefficients[0];
	sound->m_right = sound->getDSPSettings().pMatrixCoefficients[1];

	sound->getSourceVoice()->SetOutputMatrix(m_masterVoice, SOURCECHANNELS,
		m_destChannels, sound->getDSPSettings().pMatrixCoefficients);
}

void SoundWrapper::updateSound( unsigned int p_index, 
							   const AudioHeader::PlayState& p_soundInstruction )
{
	switch (p_soundInstruction)
	{
		case AudioHeader::PlayState::PLAY:
			m_sounds[p_index]->resumeOrPlay();
			break;
		case AudioHeader::PlayState::STOP:
			m_sounds[p_index]->stop();
			break;
		case AudioHeader::PlayState::PAUSE:
			m_sounds[p_index]->pause();
			break;
		case AudioHeader::PlayState::RESTART:
			m_sounds[p_index]->restart();
			break;
		case AudioHeader::PlayState::RESUME:
			m_sounds[p_index]->resumeOrPlay();
			break;
		default:
			break;
	}
}

float* SoundWrapper::getMasterVolumeRef()
{
	return &m_masterVolume;
}

float SoundWrapper::getMasterVolume() const
{
	return m_masterVolume;
}

void SoundWrapper::setMasterVolume( const float p_value )
{
	m_masterVolume = p_value;
}

void SoundWrapper::updateMasterVolume(){
	m_masterVoice->SetVolume(m_masterVolume,0);
}

bool SoundWrapper::isPlaying( const int soundIndex ){
	return m_activeSounds[soundIndex]->isPlaying();
}

Sound* SoundWrapper::getSound( int p_index ){
	return m_activeSounds[p_index];
}

unsigned int SoundWrapper::createSoundFromHeader( const AudioHeader* p_audioHeader ){
	return m_sounds.add(m_soundFactory->createSoundFromHeader(p_audioHeader));
}

void SoundWrapper::destroySound( unsigned int p_index )
{
	delete m_sounds.at(p_index);
	m_sounds.removeAt(p_index);
}

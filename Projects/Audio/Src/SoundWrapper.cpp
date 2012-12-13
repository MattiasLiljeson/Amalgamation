#include "SoundWrapper.h"


SoundWrapper::SoundWrapper()
{
	m_soundDevice	= NULL;
	m_masterVoice	= NULL;
	m_masterVolume	= 1.0f;




	/************************************************************************/
	/* XAudio2 2.7 specific call, this is called behind the scene in later	*/
	/* versions																*/
	/************************************************************************/
	CoInitializeEx( NULL, COINIT_MULTITHREADED );

	initSoundEngine();

	m_soundFactory = new SoundFactory(m_soundDevice);
}

SoundWrapper::~SoundWrapper()
{
	for (unsigned int i = 0; i <m_createdSounds.size(); i++)
	{
		delete m_createdSounds[i];
	}
	m_createdSounds.clear();

	/************************************************************************/
	/* Be sure to stop and destroy all the source voices before stopping and*/
	/* destroying the audio engine.											*/
	/************************************************************************/
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
	if ( FAILED (hr = XAudio2Create(&m_soundDevice,flags)))
		throw XAudio2Exception(hr,__FILE__,__FUNCTION__,__LINE__);

	if( FAILED( hr = m_soundDevice->CreateMasteringVoice(&m_masterVoice)))
		throw XAudio2Exception(hr,__FILE__,__FUNCTION__,__LINE__);

	/************************************************************************/
	/* Get all the juicy details about the sound device.					*/
	/************************************************************************/
	ZeroMemory(&m_details, sizeof(XAUDIO2_DEVICE_DETAILS));
	hr = m_soundDevice->GetDeviceDetails( 0, &m_details );
	if ( FAILED(hr) )
		throw XAudio2Exception(hr,__FILE__,__FUNCTION__,__LINE__);

	m_destChannels = m_details.OutputFormat.Format.nChannels;

	init3DSoundSettings();
	initListener();
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

int SoundWrapper::createNewNonPositionalSound( const char* p_filePath )
{

	m_createdSounds.push_back(m_soundFactory->createNonPositionalSound(p_filePath));
	return m_createdSounds.size()-1; // returns the newly created sound index
}

int SoundWrapper::createNewPositionalSound(const char* p_filePath, 
														const AglVector3& p_pos)
{
	m_createdSounds.push_back(m_soundFactory->createPositionalSound(p_filePath, p_pos, 
		m_emitterAzimuths));
	return m_createdSounds.size()-1; // returns the newly created sound index
}

void SoundWrapper::init3DSoundSettings()
{
	/************************************************************************/
	/* MatrixCoefficients contains the volume for each output channel. 		*/
	/* Please see MSDN for more info search "pMatrixCoefficients"			*/
	/************************************************************************/
	m_matrixCoefficients	= new FLOAT32[SOURCECHANNEL* m_destChannels];
	m_emitterAzimuths		= new FLOAT32[SOURCECHANNEL]; //UNKOWN VARIABLE

	/************************************************************************/
	/* The number of output channels have to be know before the 3D			*/
	/* calculations can be used. The speed of sound can be altered for		*/
	/* artistic feeling.													*/
	/************************************************************************/
	X3DAudioInitialize( m_details.OutputFormat.dwChannelMask, X3DAUDIO_SPEED_OF_SOUND, 
		m_x3DAudioInstance);

	ZeroMemory(&m_dspSettings,sizeof(X3DAUDIO_DSP_SETTINGS));
	m_dspSettings.SrcChannelCount = SOURCECHANNEL;
	m_dspSettings.DstChannelCount = m_destChannels;
	m_dspSettings.pMatrixCoefficients = m_matrixCoefficients;
}

void SoundWrapper::initListener()
{
	SoundSceneInfo info;
	info.listenerOrientFront	= AglVector3(1,0,0);
	info.listenerOrientTop		= AglVector3(0,1,0);
	info.listenerPos			= AglVector3(0,0,0);
	info.listenerVelocity		= AglVector3(0,0,0);

	updateListener(info);
}

void SoundWrapper::update(int p_index, bool p_positionalSound /* = false */)
{
	m_masterVoice->SetVolume(m_masterVolume,0);

	if (p_positionalSound)
	{
		/************************************************************************/
		/* UNSURE what settings should be applied here.							*/
		/************************************************************************/
		X3DAudioCalculate(m_x3DAudioInstance, &m_listener, 
			&static_cast<PositionalSound*>(m_createdSounds[p_index])->getEmitter(),
			X3DAUDIO_CALCULATE_MATRIX, &m_dspSettings);

		float left = m_matrixCoefficients[0];
		float right = m_matrixCoefficients[1];

		m_createdSounds[p_index]->getSourceVoice()->SetOutputMatrix(m_masterVoice, 1,
			m_destChannels, m_matrixCoefficients);
	}
}

void SoundWrapper::setListenerPos( AglVector3 p_newPos )
{
	m_listener.Position.x = p_newPos.x;
	m_listener.Position.y = p_newPos.y;
	m_listener.Position.z = p_newPos.z;
}

void SoundWrapper::updateSound( int p_index, 
							   const SoundEnums::Instructions& p_soundInstruction )
{
	switch (p_soundInstruction)
	{
		case SoundEnums::Instructions::PLAY:
			m_createdSounds[p_index]->resumeOrPlay();
			break;
		case SoundEnums::Instructions::PAUSE:
			m_createdSounds[p_index]->pause();
			break;
		case SoundEnums::Instructions::STOP:
			m_createdSounds[p_index]->stop();
			break;
		case SoundEnums::Instructions::RESTART:
			m_createdSounds[p_index]->restart();
			break;
		default:
			break;
	}
}

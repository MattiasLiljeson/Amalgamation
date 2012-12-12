#include "SoundWrapper.h"


SoundWrapper::SoundWrapper()
{
	m_soundDevice	= NULL;
	m_masterVoice	= NULL;
	m_masterVolume	= 0;


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
	/************************************************************************/
	/* Some of the init code has been separated from SoundWrapper, mainly	*/
	/* the XAudio2 init and not the XAudio 3D position parts.				*/
	/************************************************************************/
	m_soundDevice = AudioEngineCreator::createAudioEngine();
	m_masterVoice = AudioEngineCreator::createMasterVoice(m_soundDevice);

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

Sound* SoundWrapper::createNewNonPositionalSound( const char* p_filePath )
{
	return m_soundFactory->createNonPositionalSound(p_filePath);
}

PositionalSound* SoundWrapper::createNewPositionalSound( const char* p_filePath )
{
	return m_soundFactory->createPositionalSound(p_filePath);
}

void SoundWrapper::init3DSoundSettings()
{
	HRESULT hr = S_OK;
	/************************************************************************/
	/* Get all the juicy details about the sound device.					*/
	/************************************************************************/
	ZeroMemory(&m_details, sizeof(XAUDIO2_DEVICE_DETAILS));
	m_soundDevice->GetDeviceDetails(0,&m_details);
	hr = m_soundDevice->GetDeviceDetails( 0, &m_details );
	if ( FAILED(hr) )
		throw XAudio2Exception(hr,__FILE__,__FUNCTION__,__LINE__);

	DWORD dwChannelMask = m_details.OutputFormat.dwChannelMask;
	m_destChannels = m_details.OutputFormat.Format.nChannels;

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
	X3DAudioInitialize( dwChannelMask, X3DAUDIO_SPEED_OF_SOUND, m_x3DAudioInstance);

	ZeroMemory(&m_dspSettings,sizeof(X3DAUDIO_DSP_SETTINGS));
	m_dspSettings.SrcChannelCount = SOURCECHANNEL;
	m_dspSettings.DstChannelCount = m_destChannels;
	m_dspSettings.pMatrixCoefficients = m_matrixCoefficients;
}

void SoundWrapper::initListener()
{
	SoundSceneInfo info = {
		{0,0,0}, {0,0,1}, {0,1,0}, {0,0,0}
	};

	updateListener(info);
}

void SoundWrapper::update( PositionalSound* p_sound )
{
	/************************************************************************/
	/* UNSURE what settings should be applied here.							*/
	/************************************************************************/
	X3DAudioCalculate(m_x3DAudioInstance, &m_listener, &p_sound->getEmitter(),
		X3DAUDIO_CALCULATE_MATRIX, &m_dspSettings);

	IXAudio2SourceVoice* voice = p_sound->getSourceVoice();
	voice->SetOutputMatrix( m_masterVoice, 1, m_destChannels, m_matrixCoefficients);
}

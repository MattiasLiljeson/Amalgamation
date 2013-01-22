#include "SoundFactory.h"

SoundFactory::SoundFactory(IXAudio2* p_soundDevice)
{
	m_soundDevice = p_soundDevice;
}

SoundFactory::~SoundFactory()
{

}

Sound* SoundFactory::createAmbientSound( BasicSoundCreationInfo* p_info )
{
	XAUDIO2_BUFFER buffer = {0};
	initBuffer(&buffer, p_info);
	WAVEFORMATEX waveFormatEx;

	createSoundBuffer(p_info->getFullFilePathString().c_str(), &buffer, &waveFormatEx);
	IXAudio2SourceVoice* soundVoice = createSourceVoice(buffer, waveFormatEx);
	return new Sound(soundVoice,buffer,p_info->volume);
}

PositionalSound* SoundFactory::createPositionalSound(BasicSoundCreationInfo* p_basicSoundInfo, 
													 PositionalSoundCreationInfo* p_positionalInfo)
{
	XAUDIO2_BUFFER buffer = {0};
	WAVEFORMATEX waveFormatEx;
	initBuffer(&buffer, p_basicSoundInfo);
	createSoundBuffer(p_basicSoundInfo->getFullFilePathString().c_str(), &buffer,
		&waveFormatEx);
	X3DAUDIO_EMITTER emitter = {0};
	initEmitter(&emitter, p_positionalInfo->soundOrientation);
	X3DAUDIO_DSP_SETTINGS dspSettings = {0};
	initDSPSettings(&dspSettings,p_positionalInfo->destChannels);

	PositionalSoundInfo info;
	info.emitter	= emitter;
	info.settings	= dspSettings; 
	info.previousPosition = p_positionalInfo->soundOrientation.listenerPos;
	IXAudio2SourceVoice* soundVoice = createSourceVoice(buffer, waveFormatEx);
	return new PositionalSound( soundVoice, buffer, info, p_basicSoundInfo->volume);
}


void SoundFactory::initBuffer( XAUDIO2_BUFFER* p_audioBuffer, 
							  BasicSoundCreationInfo* p_basicSoundInfo )
{
	if (p_basicSoundInfo->loopPlayback)
		p_audioBuffer->LoopCount = XAUDIO2_LOOP_INFINITE;
	else
		p_audioBuffer->LoopCount = p_basicSoundInfo->loopPlayback;
}

void SoundFactory::initDSPSettings( X3DAUDIO_DSP_SETTINGS* p_dspSettings, int p_destChannels)
{
	p_dspSettings->SrcChannelCount = 1;
	p_dspSettings->DstChannelCount = p_destChannels;
	p_dspSettings->pMatrixCoefficients = new FLOAT32[1*p_destChannels];
}

void SoundFactory::createSoundBuffer(const char* p_fullFilePath, XAUDIO2_BUFFER* p_buffer,
									 WAVEFORMATEX* p_waveFormatEx)
{
	DWORD chunkSize, chunkPosition, fileType;

	ZeroMemory(&m_file, sizeof(HANDLE));
	initFile(p_fullFilePath);

	findChunk( m_file, fourccRIFF, chunkSize, chunkPosition);
	readChunkData(m_file, &fileType, sizeof(DWORD), chunkPosition);

	if (fileType != fourccWAVE)
		throw XAudio2Exception("Unsupported sound format",__FILE__,__FUNCTION__,__LINE__);

	/*************************************************************************/
	/* Locate the fmt chunk and copy its contents into a WAVEFORMATEXTENSIBLE*/
	/*************************************************************************/
	findChunk(m_file, fourccFMT, chunkSize, chunkPosition);
	readChunkData(m_file, p_waveFormatEx, chunkSize, chunkPosition);

	/************************************************************************/
	/* Locate the data chunk and read its contents into a buffer            */
	/************************************************************************/
	findChunk(m_file, fourccDATA, chunkSize, chunkPosition);
	BYTE* dataBuffer = new BYTE[chunkSize];
	readChunkData(m_file, dataBuffer, chunkSize, chunkPosition);

	/************************************************************************/
	/* HACK: Most options of the buffer should be sent into the function	*/
	/* rather than the hard coded way below									*/
	/************************************************************************/
	p_buffer->AudioBytes = chunkSize;
	p_buffer->pAudioData = dataBuffer;
	p_buffer->Flags = XAUDIO2_END_OF_STREAM;
	p_buffer->LoopBegin = 0;
	//p_buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
}

IXAudio2SourceVoice* SoundFactory::createSourceVoice(XAUDIO2_BUFFER& p_buffer,
													 WAVEFORMATEX& p_waveFormatEx,
													 float maxFreqOffset/* =1.0f */)
{
	/************************************************************************/
	/* Create the source voice after the buffers has been formated.			*/
	/************************************************************************/
	IXAudio2SourceVoice* soundVoice;
	UINT32 flags = 0;
	IXAudio2VoiceCallback* callback = NULL;
	XAUDIO2_VOICE_SENDS* sendList = NULL;
	XAUDIO2_EFFECT_CHAIN* effectChain = NULL;

	HRESULT hr;
	if (FAILED(hr = m_soundDevice->CreateSourceVoice( &soundVoice, 
		&p_waveFormatEx, flags , maxFreqOffset, callback, sendList, effectChain)))
		throw XAudio2Exception(hr,__FILE__,__FUNCTION__,__LINE__);
	return soundVoice;
}

void SoundFactory::findChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, 
							   DWORD& dwChunkDataPosition)
{
	HRESULT hr = S_OK;
	if( INVALID_SET_FILE_POINTER == SetFilePointer( hFile, 0, NULL, FILE_BEGIN ) )
	{
		hr = HRESULT_FROM_WIN32( GetLastError() );
		throw XAudio2Exception(hr, __FILE__,__FUNCTION__,__LINE__);
	}
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD bytesRead = 0;
	DWORD dwOffset = 0;

	while (hr == S_OK)
	{
		DWORD dwRead;
		if( 0 == ReadFile( hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL ) )
		{
			hr = HRESULT_FROM_WIN32( GetLastError() );
			throw XAudio2Exception(hr, __FILE__,__FUNCTION__,__LINE__);
		}

		if( 0 == ReadFile( hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL ) )
		{
			hr = HRESULT_FROM_WIN32( GetLastError() );
			throw XAudio2Exception(hr, __FILE__,__FUNCTION__,__LINE__);
		}

		switch (dwChunkType)
		{
		case fourccRIFF:
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if( 0 == ReadFile( hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL ) )
				hr = HRESULT_FROM_WIN32( GetLastError() );
			break;

		default:
			if( INVALID_SET_FILE_POINTER == SetFilePointer( hFile, dwChunkDataSize, 
				NULL, FILE_CURRENT ) )
			{
				hr = HRESULT_FROM_WIN32( GetLastError() );
				throw XAudio2Exception(hr, __FILE__,__FUNCTION__,__LINE__);
			}
		}

		dwOffset += sizeof(DWORD) * 2;

		if (dwChunkType == fourcc)
		{
			dwChunkSize = dwChunkDataSize;
			dwChunkDataPosition = dwOffset;
			return;
		}

		dwOffset += dwChunkDataSize;

		if (bytesRead >= dwRIFFDataSize) 
			throw XAudio2Exception("Bytes to read is larger then file",__FILE__,
			__FUNCTION__,__LINE__);
	}
}

void SoundFactory::readChunkData(HANDLE hFile, void* buffer, DWORD bufferSize, 
								   DWORD bufferOffset)
{
	HRESULT hr = S_OK;	
	DWORD dwRead;
	if( INVALID_SET_FILE_POINTER == SetFilePointer( hFile, bufferOffset, NULL, 
		FILE_BEGIN ) )
	{
		hr = HRESULT_FROM_WIN32( GetLastError() );
		throw XAudio2Exception(hr, __FILE__,__FUNCTION__,__LINE__);
	}
	if( 0 == ReadFile( hFile, buffer, bufferSize, &dwRead, NULL ) )
	{
		hr = HRESULT_FROM_WIN32( GetLastError() );
		throw XAudio2Exception(hr, __FILE__,__FUNCTION__,__LINE__);
	}
}

void SoundFactory::initFile(const char* p_filePath)
{
	HRESULT hr = S_OK;
	m_file = CreateFile(p_filePath,GENERIC_READ,FILE_SHARE_READ,NULL,
		OPEN_EXISTING,0,NULL);

	if (INVALID_HANDLE_VALUE == m_file)
	{
		hr = HRESULT_FROM_WIN32( GetLastError() );
		throw XAudio2Exception(hr,p_filePath, __FILE__,__FUNCTION__,__LINE__);
	}

	if ( INVALID_SET_FILE_POINTER == SetFilePointer(m_file,0,NULL,FILE_BEGIN) )
	{
		hr = HRESULT_FROM_WIN32( GetLastError() );
		throw XAudio2Exception(hr, __FILE__,__FUNCTION__,__LINE__);
	}
}

void SoundFactory::initEmitter(X3DAUDIO_EMITTER* p_emitter, 
							   SoundOrientation p_soundOrientation)
{
	/************************************************************************/
	/* The emitter should be able to receive any predefined audio cone, it's*/
	/* now hard coded.														*/
	/************************************************************************/
	p_emitter->pCone = NULL;
	if (p_emitter->pCone)
	{
		p_emitter->pCone->InnerAngle = 0.0f;
		p_emitter->pCone->OuterAngle = 0.0f;
		p_emitter->pCone->InnerVolume = 0.0f;
		p_emitter->pCone->OuterVolume = 1.0f;
		p_emitter->pCone->InnerLPF = 0.0f;
		p_emitter->pCone->OuterLPF = 1.0f;
		p_emitter->pCone->InnerReverb = 0.0f;
		p_emitter->pCone->OuterReverb = 1.0f;

		p_emitter->InnerRadius = 2.0f;
		p_emitter->InnerRadiusAngle = X3DAUDIO_PI/4.0f;
	}
	else
	{
		p_emitter->InnerRadius = 0.0f;
		p_emitter->InnerRadiusAngle = 0.0f;
	}
	X3DAUDIO_VECTOR pos = {
		p_soundOrientation.listenerPos[0],
		p_soundOrientation.listenerPos[1],
		p_soundOrientation.listenerPos[2]};
	X3DAUDIO_VECTOR orientFront = {
		p_soundOrientation.listenerOrientFront[0],
		p_soundOrientation.listenerOrientFront[1],
		p_soundOrientation.listenerOrientFront[2]};
	X3DAUDIO_VECTOR orientTop	= {
		p_soundOrientation.listenerOrientTop[0],
		p_soundOrientation.listenerOrientTop[1],
		p_soundOrientation.listenerOrientTop[2]};
	X3DAUDIO_VECTOR velocity	= {
		p_soundOrientation.listenerVelocity[0],
		p_soundOrientation.listenerVelocity[1],
		p_soundOrientation.listenerVelocity[2]};

	p_emitter->Position		= pos;
	p_emitter->OrientFront	= orientFront;
	p_emitter->OrientTop	= orientTop;
	p_emitter->Velocity		= velocity;
	p_emitter->ChannelCount		= 1;
	p_emitter->ChannelRadius	= 1.0f;
	p_emitter->pChannelAzimuths = new FLOAT32[1]; ///< UNKOWN VARIABLE

	p_emitter->pVolumeCurve = (X3DAUDIO_DISTANCE_CURVE*)&X3DAudioDefault_LinearCurve;
	p_emitter->pLFECurve    = (X3DAUDIO_DISTANCE_CURVE*)&Emitter_LFE_Curve;
	p_emitter->pLPFDirectCurve = NULL; // use default curve
	p_emitter->pLPFReverbCurve = NULL; // use default curve
	p_emitter->pReverbCurve    = (X3DAUDIO_DISTANCE_CURVE*)&Emitter_Reverb_Curve;
	p_emitter->CurveDistanceScaler	= 30.0f;
	p_emitter->DopplerScaler		= 1.0f;
}
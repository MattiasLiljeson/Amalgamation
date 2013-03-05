#include "SoundFactory.h"
#include <Globals.h>
#include <DebugUtil.h>

SoundFactory::SoundFactory(IXAudio2* p_soundDevice)
{
	m_soundDevice = p_soundDevice;
	/*SoundBufferAndHeader* bufferAndHeader = new SoundBufferAndHeader();
	BasicSoundCreationInfo basicInfo("default_what.wav", SOUNDROOTPATH.c_str());
	initBuffer(bufferAndHeader->buffer, &basicInfo);
	createSoundBuffer(basicInfo.getFullFilePathString().c_str(),
		bufferAndHeader->buffer, bufferAndHeader->waveFormatEx);
	m_soundBufferManager.addResource(basicInfo.fileName, bufferAndHeader);*/
}

SoundFactory::~SoundFactory()
{	
}
/*
Sound* SoundFactory::createAmbientSound( BasicSoundCreationInfo* p_info )
{
	SoundBufferAndHeader* bufferAndHeader =
		m_soundBufferManager.getResource(p_info->fileName);
	if(bufferAndHeader == NULL)
	{
		try
		{
			bufferAndHeader = new SoundBufferAndHeader();
			initBuffer(bufferAndHeader->buffer, p_info);
			createSoundBuffer(p_info->getFullFilePathString().c_str(),
				bufferAndHeader->buffer, bufferAndHeader->waveFormatEx);
			m_soundBufferManager.addResource(p_info->fileName, bufferAndHeader);
		}
		catch (XAudio2Exception& e)
		{
			DEBUGWARNING((e.what()));
			delete bufferAndHeader;
			bufferAndHeader = m_soundBufferManager.getResource("default_what.wav");
		}
	}

	IXAudio2SourceVoice* soundVoice = createSourceVoice(*bufferAndHeader->buffer,
		*bufferAndHeader->waveFormatEx);
	return new Sound(soundVoice, bufferAndHeader->buffer);
}

PositionalSound* SoundFactory::createPositionalSound(BasicSoundCreationInfo* p_basicSoundInfo,
													 PositionalSoundCreationInfo* p_positionalInfo)
{
	SoundBufferAndHeader* bufferAndHeader =
		m_soundBufferManager.getResource(p_basicSoundInfo->fileName);
	if(bufferAndHeader == NULL)
	{
		try
		{
			bufferAndHeader = new SoundBufferAndHeader();
			initBuffer(bufferAndHeader->buffer, p_basicSoundInfo);
			createSoundBuffer(p_basicSoundInfo->getFullFilePathString().c_str(),
				bufferAndHeader->buffer, bufferAndHeader->waveFormatEx);
			m_soundBufferManager.addResource(p_basicSoundInfo->fileName, bufferAndHeader);
		}
		catch (XAudio2Exception& e)
		{
			DEBUGWARNING((e.what()));
			delete bufferAndHeader;
			bufferAndHeader = m_soundBufferManager.getResource("default_what.wav");
		}
	}

	X3DAUDIO_EMITTER emitter = {0};
	initEmitter(&emitter, p_positionalInfo->soundOrientation);
	X3DAUDIO_DSP_SETTINGS dspSettings = {0};
	initDSPSettings(&dspSettings, p_positionalInfo->destChannels);
	PositionalSoundInfo info;
	info.emitter = emitter;
	info.settings = dspSettings; 
	info.previousPosition = p_positionalInfo->soundOrientation.listenerPos;
	IXAudio2SourceVoice* soundVoice = createSourceVoice(*bufferAndHeader->buffer,
		*bufferAndHeader->waveFormatEx);
	return new PositionalSound(soundVoice, bufferAndHeader->buffer, info,
		p_basicSoundInfo->volume);
}

Sound* SoundFactory::createAmbientSoundEffect( BasicSoundCreationInfo* p_info,
											  int p_soundIndex,
											  vector<Sound*>* p_sounds )
{
	SoundBufferAndHeader* bufferAndHeader =
		m_soundBufferManager.getResource(p_info->fileName);
	if(bufferAndHeader == NULL)
	{
		try
		{
			bufferAndHeader = new SoundBufferAndHeader();
			initBuffer(bufferAndHeader->buffer, p_info);
			createSoundBuffer(p_info->getFullFilePathString().c_str(),
				bufferAndHeader->buffer, bufferAndHeader->waveFormatEx);
			m_soundBufferManager.addResource(p_info->fileName, bufferAndHeader);
		}
		catch (XAudio2Exception& e)
		{
			DEBUGWARNING((e.what()));
			delete bufferAndHeader;
			bufferAndHeader = m_soundBufferManager.getResource("default_what.wav");
		}
	}
	IXAudio2SourceVoice* soundVoice = createSourceVoice(*bufferAndHeader->buffer,
		*bufferAndHeader->waveFormatEx,
		NULL);
	return new Sound(soundVoice, bufferAndHeader->buffer);
}

PositionalSound* SoundFactory::createPositionalSoundEffect(
	BasicSoundCreationInfo* p_basicSoundInfo, 
	PositionalSoundCreationInfo* p_positionalInfo, int p_soundIndex,
	vector<Sound*>* p_positionalSounds)
{
	SoundBufferAndHeader* bufferAndHeader =
		m_soundBufferManager.getResource(p_basicSoundInfo->fileName);
	if(bufferAndHeader == NULL)
	{
		try
		{
			bufferAndHeader = new SoundBufferAndHeader();
			initBuffer(bufferAndHeader->buffer, p_basicSoundInfo);
			createSoundBuffer(p_basicSoundInfo->getFullFilePathString().c_str(),
				bufferAndHeader->buffer, bufferAndHeader->waveFormatEx);
			m_soundBufferManager.addResource(p_basicSoundInfo->fileName, bufferAndHeader);
		}
		catch (XAudio2Exception& e)
		{
			DEBUGWARNING((e.what()));
			delete bufferAndHeader;
			bufferAndHeader = m_soundBufferManager.getResource("default_what.wav");
		}
	}

	X3DAUDIO_EMITTER emitter = {0};
	initEmitter(&emitter, p_positionalInfo->soundOrientation);
	X3DAUDIO_DSP_SETTINGS dspSettings = {0};
	initDSPSettings(&dspSettings, p_positionalInfo->destChannels);
	PositionalSoundInfo info;
	info.emitter = emitter;
	info.settings = dspSettings; 
	info.previousPosition = p_positionalInfo->soundOrientation.listenerPos;
	IXAudio2SourceVoice* soundVoice = createSourceVoice(*bufferAndHeader->buffer,
		*bufferAndHeader->waveFormatEx, NULL);
	return new PositionalSound(soundVoice, bufferAndHeader->buffer, info,
		p_basicSoundInfo->volume);
}

*/

Sound* SoundFactory::createSoundFromHeader( const AudioHeader* p_audioHeader )
{
	Sound* newSound = NULL;
	SoundBufferAndHeader* bufferAndHeader =
		m_soundBufferManager.getResource(p_audioHeader->file);
	if(bufferAndHeader == NULL)
	{
		try
		{
			bufferAndHeader = new SoundBufferAndHeader();
			initBuffer(bufferAndHeader->buffer, p_audioHeader);
			createSoundBuffer(p_audioHeader,bufferAndHeader);
			m_soundBufferManager.addResource(p_audioHeader->file, bufferAndHeader);
		}
		catch (XAudio2Exception& e)
		{
			DEBUGWARNING((e.what()));
			delete bufferAndHeader;
			bufferAndHeader = m_soundBufferManager.getResource("default_what.wav");
		}
	}

	switch (p_audioHeader->soundType)
	{
	case AudioHeader::AMBIENT:
		{
			IXAudio2SourceVoice* soundVoice = createSourceVoice(*bufferAndHeader->buffer,
				*bufferAndHeader->waveFormatEx);
			newSound = new Sound(soundVoice, bufferAndHeader->buffer);
			break;
		}
	case AudioHeader::AMBIENTRANGE:
		{
			IXAudio2SourceVoice* soundVoice = createSourceVoice(*bufferAndHeader->buffer,
				*bufferAndHeader->waveFormatEx);
			newSound = new Sound(soundVoice, bufferAndHeader->buffer);
			break;
		}

	case AudioHeader::POSITIONALSOUND:
		{
			X3DAUDIO_EMITTER emitter = {0};
			initEmitter(&emitter, p_audioHeader);
			X3DAUDIO_DSP_SETTINGS dspSettings = {0};
			initDSPSettings(&dspSettings, 2, p_audioHeader);
			PositionalSoundInfo info;
			info.emitter = emitter;
			info.settings = dspSettings;
			IXAudio2SourceVoice* soundVoice = createSourceVoice(*bufferAndHeader->buffer,
				*bufferAndHeader->waveFormatEx, NULL);
			newSound = new PositionalSound(soundVoice, bufferAndHeader->buffer, info);
			break;
		}
	default:
		throw XAudio2Exception("XAudio2 sound factory error: Unknown sound type",
			__FILE__,__FUNCTION__,__LINE__);
		break;
	}

	return newSound;
}

void SoundFactory::initEmitter(X3DAUDIO_EMITTER* p_emitter, const AudioHeader* p_audioHeader)
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
	X3DAUDIO_VECTOR pos = { p_audioHeader->pos.x, p_audioHeader->pos.y, 
		p_audioHeader->pos.z};

	X3DAUDIO_VECTOR orientFront = { p_audioHeader->front.x, p_audioHeader->front.y,
		p_audioHeader->front.z};

	X3DAUDIO_VECTOR orientTop	= { p_audioHeader->top.x, p_audioHeader->top.y,
		p_audioHeader->top.z};

	X3DAUDIO_VECTOR velocity	= { p_audioHeader->velocity.x, p_audioHeader->velocity.y,
		p_audioHeader->velocity.z};

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
	p_emitter->CurveDistanceScaler	= p_audioHeader->maxRange;
	p_emitter->DopplerScaler		= p_audioHeader->dopplerScalar;
}

void SoundFactory::initBuffer(XAUDIO2_BUFFER* p_audioBuffer, 
							  const AudioHeader* p_audioHeader)
{
	switch (p_audioHeader->playInterval)
	{
	case AudioHeader::ONCE:
		p_audioBuffer->LoopCount = 1;
		break;
	case AudioHeader::TIMERBASED:
		p_audioBuffer->LoopCount = 1;
		break;
	case AudioHeader::TWICE:
		p_audioBuffer->LoopCount = 2;
		break;
	case AudioHeader::FOREVER:
		p_audioBuffer->LoopCount = XAUDIO2_LOOP_INFINITE;
		break;
	default:
		throw XAudio2Exception("XAudio2 sound factory error: Unknown play interval",
			__FILE__,__FUNCTION__,__LINE__);
		break;
	}		
}

void SoundFactory::initDSPSettings( X3DAUDIO_DSP_SETTINGS* p_dspSettings, int p_destChannels)
{
	p_dspSettings->SrcChannelCount = 1;
	p_dspSettings->DstChannelCount = p_destChannels;
	p_dspSettings->pMatrixCoefficients = new FLOAT32[1*p_destChannels];
}

void SoundFactory::initDSPSettings(X3DAUDIO_DSP_SETTINGS* p_dspSettings, 
								   unsigned int p_destChannels, 
								   const AudioHeader* p_audioHeader)
{
	p_dspSettings->pDelayTimes = NULL;
	p_dspSettings->SrcChannelCount = p_audioHeader->sourceChannels;
	p_dspSettings->DstChannelCount = p_destChannels;
	p_dspSettings->pMatrixCoefficients = 
		new FLOAT32[p_dspSettings->SrcChannelCount*p_dspSettings->DstChannelCount];
	p_dspSettings->DopplerFactor = p_audioHeader->dopplerFactor;
}


void SoundFactory::createSoundBuffer(const AudioHeader* p_audioHeader, 
									 SoundBufferAndHeader* p_soundBuffer)
{
	DWORD chunkSize, chunkPosition, fileType;
	ZeroMemory(&m_file, sizeof(HANDLE));
	initFile(toString(p_audioHeader->path + p_audioHeader->file).c_str());

	findChunk( m_file, fourccRIFF, chunkSize, chunkPosition);
	readChunkData(m_file, &fileType, sizeof(DWORD), chunkPosition);

	if (fileType != fourccWAVE)
		throw XAudio2Exception("Unsupported sound format",__FILE__,__FUNCTION__,__LINE__);

	/*************************************************************************/
	/* Locate the fmt chunk and copy its contents into a WAVEFORMATEXTENSIBLE*/
	/*************************************************************************/
	findChunk(m_file, fourccFMT, chunkSize, chunkPosition);
	readChunkData(m_file, p_soundBuffer->waveFormatEx, chunkSize, chunkPosition);

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
	p_soundBuffer->buffer->AudioBytes = chunkSize;
	p_soundBuffer->buffer->pAudioData = dataBuffer;
	p_soundBuffer->buffer->Flags = XAUDIO2_END_OF_STREAM;
	p_soundBuffer->buffer->LoopBegin = 0;
	p_soundBuffer->buffer->LoopLength = 0;
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
	p_buffer->LoopLength = 0;
}

IXAudio2SourceVoice* SoundFactory::createSourceVoice(XAUDIO2_BUFFER& p_buffer,
													 WAVEFORMATEX& p_waveFormatEx,
													 IXAudio2VoiceCallback* p_callback,
													 float maxFreqOffset/* =1.0f */)
{
	/************************************************************************/
	/* Create the source voice after the buffers has been formated.			*/
	/************************************************************************/
	IXAudio2SourceVoice* soundVoice;
	UINT32 flags = 0;
	XAUDIO2_VOICE_SENDS* sendList = NULL;
	XAUDIO2_EFFECT_CHAIN* effectChain = NULL;

	HRESULT hr = m_soundDevice->CreateSourceVoice(&soundVoice, &p_waveFormatEx, flags,
		maxFreqOffset, p_callback, sendList, effectChain);
	if (FAILED(hr))
	{
		throw XAudio2Exception(hr, __FILE__, __FUNCTION__, __LINE__);
	}
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
	m_file = CreateFile(p_filePath, GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, 0, NULL);

	if (INVALID_HANDLE_VALUE == m_file)
	{
		hr = HRESULT_FROM_WIN32( GetLastError() );
		throw XAudio2Exception(hr, p_filePath, __FILE__, __FUNCTION__, __LINE__);
	}

	if ( INVALID_SET_FILE_POINTER == SetFilePointer(m_file, 0, NULL, FILE_BEGIN) )
	{
		hr = HRESULT_FROM_WIN32( GetLastError() );
		throw XAudio2Exception(hr, __FILE__, __FUNCTION__, __LINE__);
	}
}
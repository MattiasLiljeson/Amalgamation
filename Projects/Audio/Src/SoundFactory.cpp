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
	IXAudio2SourceVoice* soundVoice;
	WAVEFORMATEXTENSIBLE waveFormatEx;
	XAUDIO2_BUFFER buffer;
	ZeroMemory(&buffer, sizeof(XAUDIO2_BUFFER));
	if (p_info->loopPlayback)
		buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	else
		buffer.LoopCount = p_info->loopPlayback;

	ZeroMemory(&m_file, sizeof(HANDLE));
	initFile(p_info->file);

	soundVoice = fillBuffer(waveFormatEx,buffer);
	return new Sound(soundVoice,buffer,p_info->volume);
}


PositionalSound* SoundFactory::createPositionalSound( BasicSoundCreationInfo* p_info, 
													 const AglVector3& p_pos)
{
	IXAudio2SourceVoice* soundVoice;
	WAVEFORMATEXTENSIBLE waveFormatEx;
	XAUDIO2_BUFFER buffer;
	ZeroMemory(&buffer, sizeof(XAUDIO2_BUFFER));
	if (p_info->loopPlayback)
		buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	else
		buffer.LoopCount = p_info->loopPlayback;

	/************************************************************************/
	/* Positional sound specifics.											*/
	/************************************************************************/
	/* START */
	X3DAUDIO_EMITTER emitter = {0};
	
	initEmitter(&emitter, p_pos, NULL);

	/************************************************************************/
	/* PositionalSoundInfo should be sent into the sound factory.			*/
	/************************************************************************/
	PositionalSoundInfo info;
	info.emitter	= emitter;
	/* END */

	ZeroMemory(&m_file, sizeof(HANDLE));
	initFile(p_info->file);

	soundVoice = fillBuffer(waveFormatEx,buffer);
	return new PositionalSound(soundVoice,buffer,info,p_info->volume);
}

IXAudio2SourceVoice* SoundFactory::fillBuffer( WAVEFORMATEXTENSIBLE& p_waveFormatEx, 
							  XAUDIO2_BUFFER& p_buffer )
{
	DWORD chunkSize, chunkPosition, fileType;
	findChunk( m_file, fourccRIFF, chunkSize, chunkPosition);

	readChunkData(m_file,&fileType,sizeof(DWORD), chunkPosition);

	if (fileType != fourccWAVE)
		throw XAudio2Exception("Unsupported sound format",__FILE__,__FUNCTION__,__LINE__);

	/*************************************************************************/
	/* Locate the fmt chunk and copy its contents into a WAVEFORMATEXTENSIBLE*/
	/*************************************************************************/
	findChunk(m_file,fourccFMT,chunkSize,chunkPosition);
	readChunkData(m_file,&p_waveFormatEx,chunkSize,chunkPosition);

	/************************************************************************/
	/* Locate the data chunk and read its contents into a buffer            */
	/************************************************************************/
	findChunk(m_file, fourccDATA,chunkSize,chunkPosition);
	BYTE* dataBuffer = new BYTE[chunkSize];
	readChunkData(m_file,dataBuffer,chunkSize, chunkPosition);

	/************************************************************************/
	/* HACK: Most options of the buffer should be sent into the function	*/
	/* rather than the hard coded way below									*/
	/************************************************************************/
	p_buffer.AudioBytes = chunkSize;
	p_buffer.pAudioData = dataBuffer;
	p_buffer.Flags = XAUDIO2_END_OF_STREAM;
	p_buffer.LoopBegin = 0;
	//p_buffer.LoopCount = XAUDIO2_LOOP_INFINITE;

	/************************************************************************/
	/* Create the source voice after the buffers has been formated.			*/
	/************************************************************************/
	IXAudio2SourceVoice* soundVoice;
	HRESULT hr = m_soundDevice->CreateSourceVoice( &soundVoice, 
		(WAVEFORMATEX*)&p_waveFormatEx, 0, 1.0f, NULL, NULL, NULL);
	if (FAILED(hr))
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
	if( INVALID_SET_FILE_POINTER == SetFilePointer( hFile, bufferOffset, NULL, 
		FILE_BEGIN ) )
	{
		hr = HRESULT_FROM_WIN32( GetLastError() );
		throw XAudio2Exception(hr, __FILE__,__FUNCTION__,__LINE__);
	}
	DWORD dwRead;
	if( 0 == ReadFile( hFile, buffer, bufferSize, &dwRead, NULL ) )
	{
		hr = HRESULT_FROM_WIN32( GetLastError() );
		throw XAudio2Exception(hr, __FILE__,__FUNCTION__,__LINE__);
	}
}

void SoundFactory::initFile(string p_filePath)
{
	HRESULT hr = S_OK;
	m_file = CreateFile(p_filePath.c_str(),GENERIC_READ,FILE_SHARE_READ,NULL,
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

void SoundFactory::initEmitter(X3DAUDIO_EMITTER* p_emitter, AglVector3 p_pos, float* p_pChannelAzimuths)
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
		p_emitter->InnerRadiusAngle = X3DAUDIO_PI/4.0f;;
	}
	else
	{
		p_emitter->InnerRadius = 0.0f;
		p_emitter->InnerRadiusAngle = 0.0f;
	}
	X3DAUDIO_VECTOR pos = {p_pos[0],p_pos[1],p_pos[2]};
	X3DAUDIO_VECTOR orientFront = {0,0,1};
	X3DAUDIO_VECTOR orientTop	= {0,1,0};
	X3DAUDIO_VECTOR velocity	= {0,0,0};


	p_emitter->Position		= pos;
	p_emitter->OrientFront	= orientFront;
	p_emitter->OrientTop	= orientTop;
	p_emitter->Velocity		= velocity;
	p_emitter->ChannelCount		= 1;
	p_emitter->ChannelRadius	= 1.0f;
	p_emitter->pChannelAzimuths = p_pChannelAzimuths; ///< UNKOWN VARIABLE

	p_emitter->pVolumeCurve = (X3DAUDIO_DISTANCE_CURVE*)&X3DAudioDefault_LinearCurve;
	p_emitter->pLFECurve    = (X3DAUDIO_DISTANCE_CURVE*)&Emitter_LFE_Curve;
	p_emitter->pLPFDirectCurve = NULL; // use default curve
	p_emitter->pLPFReverbCurve = NULL; // use default curve
	p_emitter->pReverbCurve    = (X3DAUDIO_DISTANCE_CURVE*)&Emitter_Reverb_Curve;
	p_emitter->CurveDistanceScaler	= 30.0f;
	p_emitter->DopplerScaler		= 1.0f;
}
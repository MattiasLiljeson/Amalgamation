#pragma once
#include <xaudio2.h>
#include <x3daudio.h>
#include <AglVector3.h>
#include "Sound.h"
#include "PositionalSound.h"
#include "XAudio2Exception.h"
#include "SoundDefines.h"
#include "AudioCurves.h"
#include "BasicSoundCreationInfo.h"
#include "PositionalSoundCreationInfo.h"
#include <ResourceManager.h>
#include "SoundBufferAndHeader.h"
#include "AudioHeader.h"

// =======================================================================================
//                                      SoundFactory
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Creates sound buffers from sound files, and can instantiate Source Voices out
/// of the sound buffers.
///        
/// # SoundFactory
/// Please note that it currently only supports wav-files.
/// Created on: 7-12-2012 
///---------------------------------------------------------------------------------------

class SoundFactory
{
public:
	SoundFactory(IXAudio2* p_soundDevice);
	~SoundFactory();

	Sound* createSoundFromHeader(const AudioHeader* p_audioHeader);
private:
	void createSoundBuffer(const char* p_fullFilePath, XAUDIO2_BUFFER* p_buffer,
		WAVEFORMATEX* p_waveFormatEx);	
	void createSoundBuffer(const AudioHeader* p_audioHeader, 
		SoundBufferAndHeader* p_soundBuffer);
	IXAudio2SourceVoice* createSourceVoice(XAUDIO2_BUFFER& p_buffer,
		WAVEFORMATEX& p_waveFormatEx, float maxFreqOffset);

	void findChunk(HANDLE hFile, DWORD fourcc,DWORD& dwChunkSize, 
		DWORD& dwChunkDataPosition);
	void readChunkData(HANDLE hFile, void* buffer, DWORD bufferSize, DWORD bufferOffset);
	void initBuffer(XAUDIO2_BUFFER* p_audioBuffer, 
		BasicSoundCreationInfo* p_basicSoundInfo);
	void initBuffer(XAUDIO2_BUFFER* p_audioBuffer, const AudioHeader* p_audioHeader);
	void initFile(const char* p_filePath);
	void initEmitter(X3DAUDIO_EMITTER* p_emitter, SoundOrientation p_soundOrientation);
	void initEmitter(X3DAUDIO_EMITTER* p_emitter, const AudioHeader* p_audioHeader);
	void initDSPSettings(X3DAUDIO_DSP_SETTINGS* p_dspSettings, int p_destChannels);
	void initDSPSettings(X3DAUDIO_DSP_SETTINGS* p_dspSettings, unsigned int p_destChannels,
		const AudioHeader* p_audioHeader);
private:
	IXAudio2* m_soundDevice;
	HANDLE	m_file; ///< m_file is always used when loading sounds from file
	ResourceManager<SoundBufferAndHeader> m_soundBufferManager;
};

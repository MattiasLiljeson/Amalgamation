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

	///-----------------------------------------------------------------------------------
	/// Create an ambient sound that can be altered in runtime.
	/// \param p_flePath
	/// \return Sound*
	///-----------------------------------------------------------------------------------
	Sound* createAmbientSound(BasicSoundCreationInfo* p_info);

	///-----------------------------------------------------------------------------------
	/// Create an ambient sound effects that deletes itself after playback is done.
	/// \param p_info
	/// \param p_soundIndex
	/// \param p_positionalSounds
	/// \return Sound*
	///-----------------------------------------------------------------------------------
	Sound* createAmbientSoundEffect(BasicSoundCreationInfo* p_info, int p_soundIndex,
		vector<Sound*>* p_sounds);
		

	///-----------------------------------------------------------------------------------
	/// Creates a 3D positional sound that can be altered in runtime.
	/// \param p_basicSoundInfo
	/// \param p_positionalInfo
	/// \return PositionalSound*
	///-----------------------------------------------------------------------------------
	PositionalSound* createPositionalSound(BasicSoundCreationInfo* p_basicSoundInfo, 
		PositionalSoundCreationInfo* p_positionalInfo);

	///-----------------------------------------------------------------------------------
	/// Creates a 3D positional sound-effect that deletes itself after playback is done.
	/// \param p_basicSoundInfo
	/// \param p_positionalInfo
	/// \param p_soundIndex
	/// \param p_positionalSounds
	/// \return PositionalSound*
	///-----------------------------------------------------------------------------------
	PositionalSound* createPositionalSoundEffect(BasicSoundCreationInfo* p_basicSoundInfo, 
		PositionalSoundCreationInfo* p_positionalInfo, int p_soundIndex,
		vector<Sound*>* p_positionalSounds);
protected:
private:
	void createSoundBuffer(const char* p_fullFilePath, XAUDIO2_BUFFER* p_buffer,
		WAVEFORMATEX* p_waveFormatEx);
	IXAudio2SourceVoice* createSourceVoice(XAUDIO2_BUFFER& p_buffer,
		WAVEFORMATEX& p_waveFormatEx, IXAudio2VoiceCallback* p_callback = NULL,
		float maxFreqOffset=1.0f);

	void findChunk(HANDLE hFile, DWORD fourcc,DWORD& dwChunkSize, 
		DWORD& dwChunkDataPosition);
	void readChunkData(HANDLE hFile, void* buffer, DWORD bufferSize, DWORD bufferOffset);
	void initBuffer(XAUDIO2_BUFFER* p_audioBuffer, BasicSoundCreationInfo* p_basicSoundInfo);
	void initFile(const char* p_filePath);
	void initEmitter(X3DAUDIO_EMITTER* p_emitter, SoundOrientation p_soundOrientation);
	void initDSPSettings(X3DAUDIO_DSP_SETTINGS* p_dspSettings, int p_destChannels);

private:
	IXAudio2* m_soundDevice;
	HANDLE	m_file; ///< m_file is always used when loading sounds from file
	ResourceManager<SoundBufferAndHeader> m_soundBufferManager;
	vector<IXAudio2VoiceCallback*> m_addedCallbacks;
};

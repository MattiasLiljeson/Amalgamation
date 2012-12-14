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

// =======================================================================================
//                                      SoundFactory
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Handles the creation of sound files
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
	/// This function only allows for a basic non positional sound to be created, please 
	/// see other functions for various other sounds
	/// \param p_flePath
	/// \returns Sound*
	///-----------------------------------------------------------------------------------
	Sound* createAmbientSound( BasicSoundCreationInfo* p_info );

	PositionalSound* createPositionalSound( BasicSoundCreationInfo* p_basicSoundInfo, 
		PositionalSoundCreationInfo* p_positionalInfo);
protected:
private:
	IXAudio2SourceVoice* createSourceVoice(const char* p_fullFilePath, 
		XAUDIO2_BUFFER& p_buffer);
	void findChunk(HANDLE hFile, DWORD fourcc,DWORD& dwChunkSize, 
		DWORD& dwChunkDataPosition);
	void readChunkData(HANDLE hFile, void* buffer, DWORD bufferSize, DWORD bufferOffset);
	void initBuffer(XAUDIO2_BUFFER* p_audioBuffer, BasicSoundCreationInfo* p_basicSoundInfo);
	void initFile(const char* p_filePath);
	void initEmitter(X3DAUDIO_EMITTER* p_emitter, SoundOrientation p_soundOrientation);
	void initDSPSettings(X3DAUDIO_DSP_SETTINGS* p_dspSettings, int p_destChannels);
private:
	IXAudio2*	m_soundDevice;

	
	HANDLE	m_file; ///< m_file is always used when loading sounds from file
};

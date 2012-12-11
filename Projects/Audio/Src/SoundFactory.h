#pragma once
#include <xaudio2.h>
#include "Sound.h"
#include "PositionalSound.h"
#include "XAudio2Exception.h"
#include "SoundDefines.h"

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
	Sound* createNonPositionalSound( const char* p_filePath );
	PositionalSound* createPositionalSound( const char* p_filePath );
protected:
private:
	void fillBuffer(WAVEFORMATEXTENSIBLE& p_waveFormatEx, XAUDIO2_BUFFER& p_buffer);
	void findChunk(HANDLE hFile, DWORD fourcc,DWORD& dwChunkSize, 
		DWORD& dwChunkDataPosition);
	void readChunkData(HANDLE hFile, void* buffer, DWORD bufferSize, DWORD bufferOffset);
	void initFile(string p_filePath);
private:
	IXAudio2*	m_soundDevice;
	HANDLE		m_file;
};

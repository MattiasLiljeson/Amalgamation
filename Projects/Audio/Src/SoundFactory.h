#pragma once
#include <xaudio2.h>
#include "Sound.h"
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
	/// This function does only allow for wav-files
	/// \param p_flePath
	/// \returns Sound*
	///-----------------------------------------------------------------------------------
	Sound* createNonPositionalSound( const char* p_flePath);
protected:
private:
	void findChunk(HANDLE hFile, DWORD fourcc,DWORD& dwChunkSize, 
		DWORD& dwChunkDataPosition);
	void readChunkData(HANDLE hFile, void* buffer, DWORD bufferSize, DWORD bufferOffset);
	void initFile(string p_filePath);
private:
	IXAudio2*	m_soundDevice;
	HANDLE		m_file;
};

#pragma once
#include <xaudio2.h>

// =======================================================================================
//	SoundBufferAndHeader
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief description...
///        
/// # SoundBufferAndHeader
/// Detailed description...
/// Created on: 22-1-2013 
///---------------------------------------------------------------------------------------

struct SoundBufferAndHeader
{
	SoundBufferAndHeader()
	{
		buffer = new XAUDIO2_BUFFER();
		waveFormatEx = new WAVEFORMATEX();
	}
	~SoundBufferAndHeader()
	{
		delete []buffer->pAudioData;
		delete buffer;
		delete waveFormatEx;
	}

	XAUDIO2_BUFFER* buffer;
	// Header information
	WAVEFORMATEX* waveFormatEx;
};
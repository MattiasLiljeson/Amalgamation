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
	XAUDIO2_BUFFER buffer;
	WAVEFORMATEX waveFormatEx;
};
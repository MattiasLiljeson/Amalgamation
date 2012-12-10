#pragma once

// =======================================================================================
//                                      Sound
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # Sound
/// Detailed description.....
/// Created on: 7-12-2012 
///---------------------------------------------------------------------------------------
#include <XAudio2fx.h>
#include <XAudio2.h>

class Sound
{
public:
	Sound(IXAudio2SourceVoice* p_sourceVoice, const XAUDIO2_BUFFER& p_buffer);
	virtual ~Sound();
	HRESULT play();
	HRESULT stop();
private:
	XAUDIO2_BUFFER			m_buffer;
	IXAudio2SourceVoice*	m_sourceVoice;
};
#pragma once

// =======================================================================================
//                                      Sound
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Contains functionality to wrap XAudio2 source voice info.
///        
/// # Sound
/// Detailed description.....
/// Created on: 7-12-2012 
///---------------------------------------------------------------------------------------
#include <XAudio2fx.h>
#include <XAudio2.h>
#include "XAudio2Exception.h"

class Sound
{
public:
	Sound(IXAudio2SourceVoice* p_sourceVoice, XAUDIO2_BUFFER* p_buffer);
	virtual ~Sound();

	///-----------------------------------------------------------------------------------
	/// Stops the source voice and also flushes the source buffers
	/// \returns HRESULT
	///-----------------------------------------------------------------------------------
	HRESULT stop();

	///-----------------------------------------------------------------------------------
	/// Pauses the sound but doesn't clear the source buffers
	/// \returns HRESULT
	///-----------------------------------------------------------------------------------
	HRESULT pause();

	///-----------------------------------------------------------------------------------
	/// Submits the original buffer and restarts the sound
	/// \returns HRESULT
	///-----------------------------------------------------------------------------------
	HRESULT restart();

	///-----------------------------------------------------------------------------------
	/// Only asks the source voice to begin playing it's sound no matter where the 
	/// buffer pointer. If the sound has never been played the buffer location will be at
	/// the beginning.
	/// \returns HRESULT
	///-----------------------------------------------------------------------------------
	HRESULT resumeOrPlay();

	IXAudio2SourceVoice* getSourceVoice();

	///-----------------------------------------------------------------------------------
	/// Updates the current sound's states allowing to see if the sound is playing or not.
	/// \return void
	///-----------------------------------------------------------------------------------
	void updateVoiceState();

	///-----------------------------------------------------------------------------------
	/// Checks if the sound is playing determined by the state of the queued buffers or if
	/// there are non queued.
	/// \return bool
	///-----------------------------------------------------------------------------------
	bool isPlaying();

	/************************************************************************/
	/* DEBUG STUFFS!														*/
	/************************************************************************/
	//-START-
	float* getLeftChannelRef();
	float* getRightChannelRef();

public:
	float					m_left;
	float					m_right;
	//-END-
protected:
	XAUDIO2_BUFFER*			m_buffer;
	IXAudio2SourceVoice*	m_sourceVoice;
	XAUDIO2_VOICE_STATE*	m_sourceState;
};
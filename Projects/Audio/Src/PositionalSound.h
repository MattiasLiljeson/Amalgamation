#pragma once

#include <AglVector3.h>
#include "Sound.h"
#include "PositionalSoundInfo.h" 
// =======================================================================================
//                                      PositionalSound
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Contains functionality to wrap XAudio2 source voice to be utilized as 
/// positional sound.
///        
/// # PositionalSound
/// Detailed description.....
/// Created on: 11-12-2012 
///---------------------------------------------------------------------------------------

class PositionalSound : public Sound
{
	
public:
	PositionalSound(IXAudio2SourceVoice* p_sourceVoice, const XAUDIO2_BUFFER& p_buffer,
		const PositionalSoundInfo& p_info, float p_volume=1.0f);
	virtual ~PositionalSound();
	X3DAUDIO_EMITTER	getEmitter();
	void				setPosition(const AglVector3& p_newPos );
private:
	PositionalSoundInfo m_dataInfo;
};
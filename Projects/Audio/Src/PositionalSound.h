#pragma once
//#include <X3DAudio.h>
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
		const PositionalSoundInfo& p_info);
	virtual ~PositionalSound();
	const X3DAUDIO_EMITTER& getEmitter();
private:
	PositionalSoundInfo m_dataInfo;
};
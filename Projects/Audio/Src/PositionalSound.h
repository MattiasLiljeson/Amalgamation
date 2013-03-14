#pragma once

#include <AglVector3.h>
#include "Sound.h"
#include "PositionalSoundInfo.h"
#include "AudioHeader.h"
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
	PositionalSound(IXAudio2SourceVoice* p_sourceVoice, XAUDIO2_BUFFER* p_buffer,
		const PositionalSoundInfo& p_info);

	virtual ~PositionalSound();

	X3DAUDIO_EMITTER	getEmitter();

	void setPosition(const AglVector3& p_newPos );
	void setTop(const AglVector3& p_newTop );
	void setFront(const AglVector3& p_newFront );
	void setVelocity(const AglVector3& p_newVelocity );
	void updateEmitter( const AglVector3& p_orientFront, const AglVector3& p_orientTop,
		const AglVector3& p_pos, const AglVector3& p_velocity);
	void updateEmitter( const AudioHeader* p_header);
	void setRange(const float& p_range);
	X3DAUDIO_DSP_SETTINGS getDSPSettings();
	X3DAUDIO_DSP_SETTINGS* getDSPSettingsPtr();
private:
	PositionalSoundInfo m_dataInfo;
};
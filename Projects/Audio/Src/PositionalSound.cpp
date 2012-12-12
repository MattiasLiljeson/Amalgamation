#include "PositionalSound.h"

PositionalSound::PositionalSound( IXAudio2SourceVoice* p_sourceVoice, 
								 const XAUDIO2_BUFFER& p_buffer, 
								 const PositionalSoundInfo& p_info )
								 : Sound (p_sourceVoice,p_buffer)
{
	m_dataInfo = p_info;
}

PositionalSound::~PositionalSound()
{

}

const X3DAUDIO_EMITTER& PositionalSound::getEmitter()
{
	return m_dataInfo.emitter;
}

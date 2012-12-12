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

X3DAUDIO_EMITTER PositionalSound::getEmitter()
{
	return m_dataInfo.emitter;
}

void PositionalSound::setPosition( const AglVector3& p_newPos )
{
	m_dataInfo.emitter.Position.x = p_newPos.x;
	m_dataInfo.emitter.Position.y = p_newPos.y;
	m_dataInfo.emitter.Position.z = p_newPos.z;
}

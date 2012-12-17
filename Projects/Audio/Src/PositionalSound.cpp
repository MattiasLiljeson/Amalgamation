#include "PositionalSound.h"

PositionalSound::PositionalSound( IXAudio2SourceVoice* p_sourceVoice, 
								 const XAUDIO2_BUFFER& p_buffer, 
								 const PositionalSoundInfo& p_info,
								 float p_volume /* =1.0f */)
								 : Sound(p_sourceVoice,p_buffer,p_volume)
{
	m_dataInfo = p_info;
}

PositionalSound::~PositionalSound()
{
	delete m_dataInfo.settings.pMatrixCoefficients;
	delete m_dataInfo.emitter.pChannelAzimuths;
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

X3DAUDIO_DSP_SETTINGS PositionalSound::getDSPSettings()
{
	return m_dataInfo.settings;
}


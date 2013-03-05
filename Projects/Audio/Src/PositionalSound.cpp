#include "PositionalSound.h"

PositionalSound::PositionalSound( IXAudio2SourceVoice* p_sourceVoice, 
								 XAUDIO2_BUFFER* p_buffer, 
								 const PositionalSoundInfo& p_info)
								 : Sound(p_sourceVoice, p_buffer)
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

void PositionalSound::setTop( const AglVector3& p_newTop )
{
	m_dataInfo.emitter.OrientTop.x = p_newTop.x;
	m_dataInfo.emitter.OrientTop.y = p_newTop.y;
	m_dataInfo.emitter.OrientTop.z = p_newTop.z;
}

void PositionalSound::setFront( const AglVector3& p_newFront )
{
	m_dataInfo.emitter.OrientFront.x = p_newFront.x;
	m_dataInfo.emitter.OrientFront.y = 0;
	m_dataInfo.emitter.OrientFront.z = p_newFront.z;
}

void PositionalSound::setVelocity( const AglVector3& p_newVelocity )
{
	m_dataInfo.emitter.Velocity.x = p_newVelocity.x;
	m_dataInfo.emitter.Velocity.y = p_newVelocity.y;
	m_dataInfo.emitter.Velocity.z = p_newVelocity.z;
}
void PositionalSound::updateEmitter( const AglVector3& p_orientFront, 
									const AglVector3& p_orientTop, 
									const AglVector3& p_pos, 
									const AglVector3& p_velocity )
{
	setPosition(p_pos);
	setFront(p_orientFront);
	setTop(p_orientTop);
	setVelocity(p_velocity);
}

X3DAUDIO_DSP_SETTINGS PositionalSound::getDSPSettings()
{
	return m_dataInfo.settings;
}

X3DAUDIO_DSP_SETTINGS* PositionalSound::getDSPSettingsPtr()
{
	return &m_dataInfo.settings;
}


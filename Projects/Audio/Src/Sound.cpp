#include "Sound.h"


Sound::Sound(IXAudio2SourceVoice* p_sourceVoice, XAUDIO2_BUFFER* p_buffer)
{
	m_sourceVoice = p_sourceVoice;
	m_buffer = p_buffer;

	/*
	HRESULT hr = S_OK;
	hr = m_sourceVoice->SubmitSourceBuffer(m_buffer);
	if(FAILED (hr))
		throw XAudio2Exception(hr,__FILE__,__FUNCTION__,__LINE__);
	*/
	m_sourceState = new XAUDIO2_VOICE_STATE();
}

Sound::~Sound()
{
	m_sourceVoice->Stop(0);
	m_sourceVoice->DestroyVoice();
	delete m_sourceState;
}

HRESULT Sound::stop()
{
	HRESULT hr = S_OK; 

	hr = m_sourceVoice->Stop(0);
	hr = m_sourceVoice->FlushSourceBuffers();

	return hr;
}

HRESULT Sound::pause()
{
	HRESULT hr = S_OK;

	hr = m_sourceVoice->Stop(0);

	return hr;
}

HRESULT Sound::restart()
{
	HRESULT hr = S_OK;

	hr = m_sourceVoice->FlushSourceBuffers();
	hr = m_sourceVoice->SubmitSourceBuffer(m_buffer);
	hr = m_sourceVoice->Start(0);

	return hr;
}

HRESULT Sound::resumeOrPlay()
{
	HRESULT hr = S_OK;
	if(!isPlaying()){
		hr = m_sourceVoice->SubmitSourceBuffer(m_buffer);
	}
	hr = m_sourceVoice->Start(0);
	return hr;
}

IXAudio2SourceVoice* Sound::getSourceVoice()
{
	return m_sourceVoice;
}

void Sound::updateVoiceState()
{
	m_sourceVoice->GetState(m_sourceState);
}

bool Sound::isPlaying()
{
	updateVoiceState();
	if (m_sourceState->BuffersQueued > 0 && m_sourceState->SamplesPlayed != 0)
	{
		return true;
	}
	return false;
}

float* Sound::getLeftChannelRef()
{
	return &m_left;
}

float* Sound::getRightChannelRef()
{
	return &m_right;
}

void Sound::setFrequency( float p_frequency )
{
	m_sourceVoice->SetFrequencyRatio(p_frequency);
}



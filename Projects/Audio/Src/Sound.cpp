#include "Sound.h"


Sound::Sound(IXAudio2SourceVoice* p_sourceVoice, const XAUDIO2_BUFFER& p_buffer, 
			 float p_volume/* =1.0f */)
{
	m_sourceVoice = p_sourceVoice;
	m_buffer = p_buffer;
	m_volume = p_volume;

	HRESULT hr = S_OK;
	hr = m_sourceVoice->SubmitSourceBuffer(&m_buffer);
	if(FAILED (hr))
		throw XAudio2Exception(hr,__FILE__,__FUNCTION__,__LINE__);

	m_sourceVoice->SetVolume(m_volume);
	m_sourceState = new XAUDIO2_VOICE_STATE();
}

Sound::~Sound()
{
	m_sourceVoice->Stop(0);
	m_sourceVoice->DestroyVoice();
	delete m_buffer.pAudioData;
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

	hr = m_sourceVoice->SubmitSourceBuffer(&m_buffer);
	hr = m_sourceVoice->Start(0);

	return hr;
}

HRESULT Sound::resumeOrPlay()
{
	HRESULT hr = S_OK;
	if(m_sourceState->BuffersQueued == 0)
		hr = m_sourceVoice->SubmitSourceBuffer(&m_buffer);
	hr = m_sourceVoice->Start(0);
	return hr;
}

IXAudio2SourceVoice* Sound::getSourceVoice()
{
	return m_sourceVoice;
}

XAUDIO2_VOICE_STATE* Sound::getSourceCurrentState()
{
	m_sourceVoice->GetState(m_sourceState);
	return m_sourceState;
}

void Sound::updateVoiceState()
{
	m_sourceVoice->GetState(m_sourceState);
}

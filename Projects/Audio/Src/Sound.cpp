#include "Sound.h"


Sound::Sound( IXAudio2SourceVoice* p_sourceVoice, const XAUDIO2_BUFFER& p_buffer )
{
	m_sourceVoice = p_sourceVoice;
	m_buffer = p_buffer;
}

Sound::~Sound()
{
	m_sourceVoice->DestroyVoice();
}

HRESULT Sound::play()
{
	HRESULT hr = S_OK;
	hr = m_sourceVoice->FlushSourceBuffers();
	hr = m_sourceVoice->SubmitSourceBuffer(&m_buffer);
	hr = m_sourceVoice->Start(0);

	return hr;
}

HRESULT Sound::stop()
{
	HRESULT hr = S_OK;

	hr = m_sourceVoice->Stop(0);

	return hr;
}

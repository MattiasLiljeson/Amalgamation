#include "Sound.h"


Sound::Sound( IXAudio2SourceVoice* p_sourceVoice, const XAUDIO2_BUFFER& p_buffer )
{
	m_sourceVoice = p_sourceVoice;
	m_buffer = p_buffer;

	HRESULT hr = S_OK;
	hr = m_sourceVoice->SubmitSourceBuffer(&m_buffer);
	if(FAILED (hr))
		throw XAudio2Exception(hr,__FILE__,__FUNCTION__,__LINE__);
}

Sound::~Sound()
{
	m_sourceVoice->DestroyVoice();
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
	hr = m_sourceVoice->Start(0);
	return hr;

}

#pragma once
#include <XAudio2.h>
#include <vector>
#include "Sound.h"
using namespace std;

// =======================================================================================
//	SoundDeleteAfterPlayback
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief description...
///        
/// # SoundDeleteAfterPlayback
/// Detailed description...
/// Created on: 24-1-2013 
///---------------------------------------------------------------------------------------

class SoundDeleteAfterPlayback: public IXAudio2VoiceCallback
{
public:
	SoundDeleteAfterPlayback(int p_soundIndex, vector<Sound*>* p_sounds,
		vector<IXAudio2VoiceCallback*>* p_finishedCallbacks)
	{
		m_soundIndex = p_soundIndex;
		m_sounds = p_sounds;
		m_finishedCallbacks = p_finishedCallbacks;
	}
	~SoundDeleteAfterPlayback() { }

	void __stdcall OnBufferEnd(void* pBufferContext)
	{
//		m_finishedCallbacks->push_back(this);
		delete (*m_sounds)[m_soundIndex];
		(*m_sounds)[m_soundIndex] = NULL;
//		m_sounds->erase(m_sounds->begin() + m_soundIndex);
		int a = 0;
	}
	void __stdcall OnBufferStart(void* pBufferContext) { }
	void __stdcall OnLoopEnd(void* pBufferContext) { }
	void __stdcall OnStreamEnd()
	{
		int a = 0;
	}
	void __stdcall OnVoiceError(void* pBufferContext, HRESULT Error) { }
	void __stdcall OnVoiceProcessingPassEnd()
	{
		int a = 0;
	}
	void __stdcall OnVoiceProcessingPassStart(UINT32 BytesRequired)
	{
		int a = 0;
	}

private:
	int m_soundIndex;
	vector<Sound*>* m_sounds;
	vector<IXAudio2VoiceCallback*>* m_finishedCallbacks;
};
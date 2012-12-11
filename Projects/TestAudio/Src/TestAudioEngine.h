#include <SoundWrapper.h>
#include <DebugUtil.h>
#include <exception>

using namespace igloo;
using namespace std;

Describe(A_AudioManager)
{
	SoundWrapper* m_soundEngine;
	Sound* m_sound;

	void SetUp()
	{
		try
		{
			m_soundEngine = new SoundWrapper();
			m_sound = m_soundEngine->createNewNonPositionalSound(
				"Assets/Sound/Music/Test/MusicMono.wav");
		}
		catch (exception* e)
		{
			DEBUGPRINT((e->what()));
		}
	}
	void TearDown()
	{
		delete m_sound;
		delete m_soundEngine;
	}
	It(Should_be_able_to_play_nonpositional_sound)
	{
		HRESULT hr = S_OK;
		Assert::That(hr,Equals(m_sound->resumeOrPlay() ) );
	}
	It(Should_be_able_to_play_pause_and_resume_sound)
	{
		HRESULT hr = S_OK;
		int sleepTime = 1500;
		Assert::That(hr, Equals ( m_sound->resumeOrPlay() ) );
		Sleep(sleepTime);
		Assert::That(hr, Equals ( m_sound->pause() ) );
		Sleep(sleepTime);
		Assert::That(hr, Equals ( m_sound->resumeOrPlay() ) );
		Sleep(sleepTime);
	}
	It(Should_be_able_to_play_stop_and_play)
	{
		HRESULT hr = S_OK;
		int sleepTime = 1500;
		Assert::That(hr, Equals ( m_sound->resumeOrPlay() ) );
		Sleep(sleepTime);
		Assert::That(hr, Equals ( m_sound->stop() ) );
		Sleep(sleepTime);
		Assert::That(hr, Equals ( m_sound->restart() ) );
		Sleep(sleepTime);
	}
};
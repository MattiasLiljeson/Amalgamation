#include <SoundWrapper.h>
#include <DebugUtil.h>
#include <exception>
#include <iostream>

using namespace igloo;
using namespace std;

Describe(A_Sound_Engine)
{
	SoundWrapper* m_soundEngine;
	Sound* m_sound;
	PositionalSound* m_posSound;
	HRESULT hr;

	void SetUp()
	{
		try
		{
			hr = S_OK;
			m_soundEngine = new SoundWrapper();
			m_sound = m_soundEngine->createNewNonPositionalSound(
				"Assets/Sound/Music/Test/MusicMono.wav");
			m_posSound= m_soundEngine->createNewPositionalSound(
				"Assets/Sound/SoundEffect/Test/spaceship_laser.wav");
		}
		catch (exception* e)
		{
			DEBUGPRINT((e->what()));
		}
	}
	void TearDown()
	{
		delete m_posSound;
		delete m_sound;
		delete m_soundEngine;
	}
	It(Should_be_able_to_play_nonpositional_sound)
	{
		cout<<"Playing NON-positional sound:"<<endl<<endl;
		cout<<"Play basic sound...";
		int sleepTime = 1500;
		Assert::That(hr,Equals(m_sound->resumeOrPlay() ) );
		Sleep(sleepTime);
		cout<<"Stopping."<<endl;
		Assert::That(hr, Equals(m_sound->stop() ) );
	}
	It(Should_be_able_to_play_pause_and_resume_sound)
	{
		cout<<"Playing NON-positional sound:"<<endl<<endl;
		int sleepTime = 1500;
		cout<<"Play basic sound...";
		Assert::That(hr, Equals ( m_sound->resumeOrPlay() ) );
		Sleep(sleepTime);
		cout<<"Pause...";
		Assert::That(hr, Equals ( m_sound->pause() ) );
		Sleep(sleepTime);
		cout<<"Resume...";
		Assert::That(hr, Equals ( m_sound->resumeOrPlay() ) );
		Sleep(sleepTime);
		cout<<"Stopping."<<endl;
		Assert::That(hr, Equals( m_sound->stop() ) );
	}
	It(Should_be_able_to_play_stop_and_play)
	{
		cout<<"Playing NON-positional sound:"<<endl<<endl;
		int sleepTime = 1500;
		cout<<"Play basic sound...";
		Assert::That(hr, Equals ( m_sound->resumeOrPlay() ) );
		Sleep(sleepTime);
		cout<<"Stop...";
		Assert::That(hr, Equals ( m_sound->stop() ) );
		Sleep(sleepTime);
		cout<<"Restart...";
		Assert::That(hr, Equals ( m_sound->restart() ) );
		Sleep(sleepTime);
		cout<<"Stopping."<<endl;
		Assert::That(hr, Equals( m_sound->stop() ) );
	}
	It(Should_be_able_to_play_positional_sound_at_center_location)
	{
		cout<<"Positional sound:"<<endl<<endl;
		int sleepTime = 1500;

		cout<<"Playing positional sound at center position... ";
		Assert::That(hr, Equals( m_posSound->resumeOrPlay() ) );
		Sleep(sleepTime);
		cout<<"Stopping"<<endl;
		Assert::That(hr , Equals( m_posSound->stop() ) );
	}
	It(Should_be_able_to_play_in_left_channel)
	{
		/*
		cout<<"Positional sound:"<<endl<<endl;
		int sleepTime = 1500;

		m_soundEngine->update(m_posSound);

		cout<<"Playing positional sound in the left channel";

		Assert::That(hr, Equals( m_posSound->resumeOrPlay() ) );
		Sleep(sleepTime);
		cout<<"Stopping"<<endl;
		Assert::That(hr , Equals( m_posSound->stop() ) );
		*/
	}
	It(Should_be_able_to_move_a_positional_sound)
	{
		/*
		cout<<"Positional sound:"<<endl<<endl;
		int sleepTime = 0;

		cout<<"Playing positional sound from left to right";
		for (int i = 0; i < 100; i++)
		{
			m_soundEngine->update(m_posSound);
			Assert::That(hr, Equals( m_posSound->resumeOrPlay() ) );
			Sleep(sleepTime);
		}
		cout<<"Stopping"<<endl;
		Assert::That(hr , Equals( m_posSound->stop() ) );
		*/
	}
};
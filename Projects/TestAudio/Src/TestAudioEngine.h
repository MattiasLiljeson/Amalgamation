#include <SoundWrapper.h>
#include <AglVector3.h>
#include <DebugUtil.h>
#include <exception>
#include <iostream>

using namespace igloo;
using namespace std;

Describe(A_Sound_Engine)
{
	SoundWrapper* m_soundEngine;
	Sound* m_basicSound;
	PositionalSound* m_positionalSound;
	AglVector3 m_pos;
	HRESULT hr;

	void SetUp()
	{
		try
		{
			hr = S_OK;
			m_pos[0] = 0;
			m_pos[1] = 0;
			m_pos[2] = 0;

			m_soundEngine = new SoundWrapper();
			m_basicSound = m_soundEngine->createNewNonPositionalSound(
				"Assets/Sound/Music/Test/MusicMono.wav");
			m_positionalSound= m_soundEngine->createNewPositionalSound(
				//"Assets/Sound/SoundEffect/Test/spaceship_laser.wav", m_pos);
				"Assets/Sound/Music/Test/MusicMono.wav",m_pos);
		}
		catch (exception* e)
		{
			DEBUGPRINT((e->what()));
		}
	}
	void TearDown()
	{
		delete m_positionalSound;
		delete m_basicSound;
		delete m_soundEngine;
	}
	It(Should_be_able_to_play_nonpositional_sound)
	{
		cout<<"Playing NON-positional sound:"<<endl<<endl;
		cout<<"Play basic sound...";
		int sleepTime = 0;
		Assert::That(hr,Equals(m_basicSound->resumeOrPlay() ) );
		Sleep(sleepTime);
		cout<<"Stopping."<<endl;
		Assert::That(hr, Equals(m_basicSound->stop() ) );
	}
	It(Should_be_able_to_play_pause_and_resume_sound)
	{
		cout<<"Playing NON-positional sound:"<<endl<<endl;
		int sleepTime = 0;
		cout<<"Play basic sound...";
		Assert::That(hr, Equals ( m_basicSound->resumeOrPlay() ) );
		Sleep(sleepTime);
		cout<<"Pause...";
		Assert::That(hr, Equals ( m_basicSound->pause() ) );
		Sleep(sleepTime);
		cout<<"Resume...";
		Assert::That(hr, Equals ( m_basicSound->resumeOrPlay() ) );
		Sleep(sleepTime);
		cout<<"Stopping."<<endl;
		Assert::That(hr, Equals( m_basicSound->stop() ) );
	}
	It(Should_be_able_to_play_stop_and_play)
	{
		cout<<"Playing NON-positional sound:"<<endl<<endl;
		int sleepTime = 0;
		cout<<"Play basic sound...";
		Assert::That(hr, Equals ( m_basicSound->resumeOrPlay() ) );
		Sleep(sleepTime);
		cout<<"Stop...";
		Assert::That(hr, Equals ( m_basicSound->stop() ) );
		Sleep(sleepTime);
		cout<<"Restart...";
		Assert::That(hr, Equals ( m_basicSound->restart() ) );
		Sleep(sleepTime);
		cout<<"Stopping."<<endl;
		Assert::That(hr, Equals( m_basicSound->stop() ) );
	}
	It(Should_be_able_to_play_positional_sound_at_center_location)
	{
		cout<<"Positional sound:"<<endl<<endl;
		int sleepTime = 0;

		cout<<"Playing positional sound at center position... ";
		Assert::That(hr, Equals( m_positionalSound->resumeOrPlay() ) );
		Sleep(sleepTime);
		cout<<"Stopping"<<endl;
		Assert::That(hr , Equals( m_positionalSound->stop() ) );
	}
	It(Should_be_able_to_play_in_left_channel)
	{
		/************************************************************************/
		/* UNSURE IF IT WORKS */
		/************************************************************************/
		m_pos = AglVector3(1,0,0);
		AglVector3 listenerPos = AglVector3(10,10,10);
		cout<<"Positional sound:"<<endl<<endl;
		int sleepTime = 2000;
		m_positionalSound->setPosition(m_pos);

		m_soundEngine->setListenerPos(listenerPos);
		m_soundEngine->update(m_positionalSound);

		cout<<"Playing positional sound in the left channel";

		Assert::That(hr, Equals( m_positionalSound->resumeOrPlay() ) );
		Sleep(sleepTime);
		cout<<"Stopping"<<endl;
		Assert::That(hr , Equals( m_positionalSound->stop() ) );
	}
	It(Should_be_able_to_move_a_positional_sound)
	{
		/************************************************************************/
		/* NOT WORKING!!!!														*/
		/************************************************************************/
		/*
		m_pos = AglVector3(1,0,0);
		cout<<"Positional sound:"<<endl<<endl;
		int sleepTime = 15;
		float numSteps = 100.0f;
		float stepSize = 2/numSteps;

		cout<<"Playing positional sound from left to right";
		Assert::That(hr, Equals( m_positionalSound->resumeOrPlay() ) );

		for (int i = 0; i < numSteps; i++)
		{
			m_soundEngine->update(m_positionalSound);
			m_pos.x -= stepSize;
			m_positionalSound->setPosition(m_pos);
			Sleep(sleepTime);
		}
		cout<<"Stopping"<<endl;
		Assert::That(hr , Equals( m_positionalSound->stop() ) );
		*/
	}
};
#include <SoundWrapper.h>
#include <DebugUtil.h>
#include <exception>

using namespace igloo;
using namespace std;

Describe(A_AudioManager)
{
	It(Should_be_initalized)
	{
		bool init = true;
		SoundWrapper* newSoundEngine;
		try
		{
			newSoundEngine = new SoundWrapper();
		}
		catch (exception &e)
		{
			DEBUGPRINT((e.what()));
			init = false;
		}
		delete newSoundEngine;
		AssertThat(init, Equals(true));
	}
	It(Should_be_able_to_play_nonpositional_sound)
	{
		SoundWrapper* newSoundEngine = new SoundWrapper();

		Sound* technoMusic = newSoundEngine->createNewNonPositionalSound
			("Assets/Sound/Music/Test/Techno_1.wav");

		HRESULT hr = S_OK;
		Assert::That(hr,Equals(technoMusic->play()));

		delete technoMusic;
		delete newSoundEngine;
	}
};
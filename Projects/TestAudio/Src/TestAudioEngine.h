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
		try
		{
			SoundWrapper* newSoundEngine = new SoundWrapper();
		}
		catch (exception &e)
		{
			DEBUGPRINT((e.what()));
			init = false;
		}
		AssertThat(init, Equals(true));
	}
};
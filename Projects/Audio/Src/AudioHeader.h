#include <AglVector3.h>
#include <string>

struct AudioHeader{
	enum PlayInterval{
		ONCE, FOREVER
	};

	enum PlayState{
		PLAY, STOP, PAUSE, RESET, RESUME
	};

	enum SoundType{
		AMBIENT, AMBIENTRANGE, POSITIONALSOUND
	};

	float maxRange;
	float minRange;
	float range;
	unsigned sourceChannels;
	AglVector3 pos;
	AglVector3 front;
	AglVector3 top;
	AglVector3 velocity;
	PlayState playingState;
	PlayState queuedPlayingState;
	SoundType soundType;
	std::string path;
	std::string file;
};
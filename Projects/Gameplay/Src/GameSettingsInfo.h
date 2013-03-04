#include <string>

using namespace std;

struct GameSettingsInfo
{
	int windowed; // Treated as bool
	int useHdr; // Treated as bool
	int screenHeight;
	int screenWidth;
	int masterVolume;
	int sfxVolume;
	int musicVolume;
	int rumble; // Treated as bool
	string playerName;
	string ip;
	string port;
	int enableTesselation;
	int enableSSAO;
	int enableCheats;
	int enableVSYNC;

	GameSettingsInfo()
	{
		windowed		= -1;
		useHdr			= -1;
		screenHeight	= -1;
		screenWidth		= -1;
		masterVolume	= -1;
		sfxVolume		= -1;
		musicVolume		= -1;
		rumble			= -1;
		playerName		= "";
		ip				= "";
		port			= "";
		enableTesselation  = -1;
		enableSSAO		= -1;
		enableCheats	= -1;
		enableVSYNC		= -1;
	}
};
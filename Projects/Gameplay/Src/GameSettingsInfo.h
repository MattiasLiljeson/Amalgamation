#include <string>

using namespace std;

struct GameSettingsInfo
{
	// Gfx settings
	int screenHeight;
	int screenWidth;
	int windowed;			// Treated as bool
	int useHdr;				// Treated as bool
	int enableTesselation;	// Treated as bool
	int enableSSAO;			// Treated as bool
	int enableVSYNC;		// Treated as bool

	// Sound settings
	int masterVolume;
	int sfxVolume;
	int musicVolume;

	// Network settings
	string playerName;
	string ip;
	string port;

	// Game settings
	int rumble;				// Treated as bool
	int enableCheats;		// Treated as bool

	GameSettingsInfo()
	{
		// Gfx settings
		screenHeight		= 720;
		screenWidth			= 1280;
		windowed			= true;
		useHdr				= false;
		enableTesselation	= false;
		enableSSAO			= true;
		enableVSYNC			= false;

		// Sound settings
		masterVolume		= 75;		// Percentage, %
		sfxVolume			= 75;		// Percentage, %
		musicVolume			= 75;		// Percentage, %
		
		// Network settings
		playerName			= "Bicen";
		ip					= "194.47.150.30";
		port				= "1337";

		// Game settings
		enableCheats		= true;
		rumble				= true;
	}
};
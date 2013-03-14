#pragma once

#include <string>

using namespace std;

struct GameSettingsInfo
{
	// Gfx settings
	int screenHeight;
	int screenWidth;
	int windowed;			// Treated as bool
	int enableHdr;				// Treated as bool
	int enableTesselation;	// Treated as bool
	int enableEffects;			// Treated as bool
	int enableVSYNC;		// Treated as bool

	// Sound settings
	int masterVolume;
	int sfxVolume;
	int musicVolume;

	// Network settings
	string playerName;
	string serverName;
	string ip;
	string port;
	int defaultGameTime;

	// Game settings
	int rumble;				// Treated as bool
	int enableCheats;		// Treated as bool

	GameSettingsInfo()
	{
		// Gfx settings
		screenHeight		= 720;
		screenWidth			= 1280;
		windowed			= true;
		enableHdr				= false;
		enableTesselation	= false;
		enableEffects			= true;
		enableVSYNC			= false;

		defaultGameTime		= 600; 

		// Sound settings
		masterVolume		= 75;		// Percentage, %
		sfxVolume			= 75;		// Percentage, %
		musicVolume			= 75;		// Percentage, %
		
		// Network settings
		playerName			= "PlayerName";
		serverName			= "FunHouse";
		ip					= "194.47.150.30";
		port				= "1337";

		// Game settings
		enableCheats		= true;
		rumble				= true;
	}
};
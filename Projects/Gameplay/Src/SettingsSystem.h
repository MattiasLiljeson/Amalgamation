#pragma once

#include <string>
#include <fstream>
#include <EntitySystem.h>

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
	}
};

class SettingsSystem : public EntitySystem
{
public:
	enum FileStatus {
		FileStatus_NOT_INITILIZED = -1,
		FileStatus_OK,
		FileStatus_FILE_NOT_FOUND,
		FileStatus_FILE_CANT_BE_CREATED,
		FileStatus_WRONG_FILE_STRUCTURE,
		FileStatus_CNT
	};

	SettingsSystem();
	virtual ~SettingsSystem();

	FileStatus readSettingsFile( string p_filePath = "settings.txt" );
	FileStatus writeSettingsFile( string p_filePath = "settings.txt" );

	GameSettingsInfo getSettings();
	void setSettings( GameSettingsInfo p_settings );

private:
	FileStatus checkFileReadOperation( istream* p_fileStream );
	FileStatus checkFileWriteOperation( ostream* p_fileStream );
private:
	bool m_settingsAreSet;
	GameSettingsInfo m_settings;

};


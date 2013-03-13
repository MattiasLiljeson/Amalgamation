#pragma once

#include <string>
#include <fstream>
#include <EntitySystem.h>
#include "GameSettingsInfo.h"

using namespace std;

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
	~SettingsSystem();

	FileStatus readSettingsFile( string p_path, string p_file = "settings.settings");
	FileStatus writeSettingsFile( string p_path, string p_file = "settings.settings");

	GameSettingsInfo getSettings();
	GameSettingsInfo* getSettingsRef();
	void setSettings( GameSettingsInfo p_settings );

private:
	FileStatus checkFileReadOperation( istream* p_fileStream );
	FileStatus checkFileWriteOperation( ostream* p_fileStream );
private:
	bool m_settingsAreSet;
	GameSettingsInfo m_settings;

};


#include "SettingsSystem.h"

// THIS COMMENT IS HERE TO LET GIT KNOW THAT THIS FILE HAS BEEN UPDATED!

SettingsSystem::SettingsSystem() : EntitySystem( SystemType::SettingsSystem )
{
	m_settingsAreSet = false;
}

SettingsSystem::~SettingsSystem()
{
}

SettingsSystem::FileStatus SettingsSystem::readSettingsFile(string p_path, string p_file 
															/* = "settings.txt" */)
{
	FileStatus status = FileStatus_OK;

	ifstream inFile;
	inFile.open( p_path+p_file );
	if( checkFileReadOperation( &inFile ) != FileStatus_OK ) 
	{
		string filePath = p_path + "stdSettings.settings";
		inFile.open( filePath );
	}

	if( checkFileReadOperation( &inFile ) == FileStatus_OK ) 
	{
		string ignore;
		// Gfx settings
		inFile >> ignore;
		inFile >> m_settings.screenWidth;
		inFile >> ignore;
		inFile >> m_settings.screenHeight;
		inFile >> ignore;
		inFile >> m_settings.windowed;
		inFile >> ignore;
		inFile >> m_settings.enableHdr;
		//inFile >> ignore;
		//inFile >> m_settings.enableTesselation;
		inFile >> ignore;
		inFile >> m_settings.enableEffects;
		inFile >> ignore;
		inFile >> m_settings.enableVSYNC;
		inFile >> ignore;
		inFile >> m_settings.enabledGPUMeasure;
		inFile >> ignore;
		inFile >> m_settings.enableViewportAutoResize;

		// Sound settings
		inFile >> ignore;
		inFile >> m_settings.masterVolume;
		//inFile >> ignore;
		//inFile >> m_settings.sfxVolume;
		//inFile >> ignore;
		//inFile >> m_settings.musicVolume;

		// Network settings
		inFile >> ignore;
		inFile >> m_settings.playerName;
		inFile >> ignore;
		inFile >> m_settings.ip;
		inFile >> ignore;
		inFile >> m_settings.serverName;
		inFile >> ignore;
		inFile >> m_settings.port;
		inFile >> ignore;
		inFile >> m_settings.defaultGameTime;

		// Game settings
		inFile >> ignore;
		inFile >> m_settings.rumble;
		inFile >> ignore;
		inFile >> m_settings.enableCheats;

		status = checkFileReadOperation( &inFile );
		m_settingsAreSet = true;
	} 
	else 
	{
		status =  FileStatus_FILE_NOT_FOUND;
	}

	inFile.close();

	return status;
}


SettingsSystem::FileStatus SettingsSystem::writeSettingsFile(string p_path, string p_file 
															 /* = "settings.cfg" */)
{
	FileStatus status = FileStatus_OK;

	ofstream outFile;
	outFile.open( p_path + p_file );

	if( checkFileWriteOperation( &outFile ) != FileStatus_OK ) {
		status =  FileStatus_FILE_CANT_BE_CREATED;
	} else {
		// Gfx settings
		outFile << "ScreenWidth= ";
		outFile << m_settings.screenWidth		<< "\n";
		outFile << "ScreenHeight= ";
		outFile << m_settings.screenHeight		<< "\n";
		outFile << "Windowed= ";
		outFile << m_settings.windowed			<< "\n";
		outFile << "HDRLights= ";
		outFile << m_settings.enableHdr			<< "\n";
		//outFile << "EnableTesselation= ";
		//outFile << m_settings.enableTesselation	<< "\n";
		outFile << "EnableEffects= ";
		outFile << m_settings.enableEffects		<< "\n";
		outFile << "EnableVSYNC= ";
		outFile << m_settings.enableVSYNC		<< "\n";
		outFile << "EnableGPUMeasure= ";
		outFile << m_settings.enabledGPUMeasure	<< "\n";
		outFile << "EnableViewportAutoResize= ";
		outFile << m_settings.enableViewportAutoResize	<< "\n";

		// Sound settings
		outFile << "MasterVolume= ";
		outFile << m_settings.masterVolume		<< "\n";
		//outFile << "SoundEffectVolume= ";
		//outFile << m_settings.sfxVolume			<< "\n";
		//outFile << "MusicVolume= ";
		//outFile << m_settings.musicVolume		<< "\n";

		// Networks settings
		outFile << "PlayerName= ";
		outFile << m_settings.playerName		<< "\n";
		outFile << "LastUsedIP= ";
		outFile << m_settings.ip				<< "\n";
		outFile << "LastUsedServerName= ";
		outFile << m_settings.serverName		<< "\n";
		outFile << "LastUsedPort= ";
		outFile << m_settings.port				<< "\n";
		outFile << "DefaultGameTime= ";
		outFile << m_settings.defaultGameTime	<< "\n";

		// Game settings
		outFile << "Rumble= ";
		outFile << m_settings.rumble			<< "\n";
		outFile << "EnableCheats= ";
		outFile << m_settings.enableCheats		<< "\n";

		status = checkFileWriteOperation( &outFile );
		m_settingsAreSet = true;
	}
	outFile.close();

	return status;
}

GameSettingsInfo SettingsSystem::getSettings()
{
	return m_settings;
}

void SettingsSystem::setSettings( GameSettingsInfo p_settings )
{
	m_settings = p_settings;
}

GameSettingsInfo* SettingsSystem::getSettingsRef()
{
	return &m_settings;
}

SettingsSystem::FileStatus SettingsSystem::checkFileReadOperation( istream* p_fileStream )
{
	FileStatus status = FileStatus_OK;
	if( !p_fileStream->good() ) {
		status =  FileStatus_FILE_NOT_FOUND;
	}
	return status;
}

SettingsSystem::FileStatus SettingsSystem::checkFileWriteOperation( ostream* p_fileStream )
{
	FileStatus status = FileStatus_OK;
	if( !p_fileStream->good() ) {
		status =  FileStatus_FILE_NOT_FOUND;
	}
	return status;
}

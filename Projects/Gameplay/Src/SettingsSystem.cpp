#include "SettingsSystem.h"

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
	string temp;
	if( checkFileReadOperation( &inFile ) != FileStatus_OK ) {
		status =  FileStatus_FILE_NOT_FOUND;
	} else {
		inFile >> temp;
		inFile >> m_settings.windowed;
		inFile >> temp;
		inFile >> m_settings.useHdr;
		inFile >> temp;
		inFile >> m_settings.screenHeight;
		inFile >> temp;
		inFile >> m_settings.screenWidth;
		inFile >> temp;
		inFile >> m_settings.masterVolume;
		inFile >> temp;
		inFile >> m_settings.sfxVolume;
		inFile >> temp;
		inFile >> m_settings.musicVolume;
		inFile >> temp;
		inFile >> m_settings.rumble;
		inFile >> temp;
		inFile >> m_settings.playerName;
		inFile >> temp;
		inFile >> m_settings.ip;
		inFile >> temp;
		inFile >> m_settings.port;
		inFile >> temp;
		inFile >> m_settings.enableTesselation;
		inFile >> temp;
		inFile >> m_settings.enableSSAO;
		inFile >> temp;
		inFile >> m_settings.enableCheats;
		inFile >> temp;
		inFile >> m_settings.enableVSYNC;

		status = checkFileReadOperation( &inFile );
		m_settingsAreSet = true;
	}

	inFile.close();

	return status;
}


SettingsSystem::FileStatus SettingsSystem::writeSettingsFile(string p_path, string p_file 
															 /* = "settings.txt" */)
{
	FileStatus status = FileStatus_OK;

	ofstream outFile;
	outFile.open( p_path + p_file );

	if( checkFileWriteOperation( &outFile ) != FileStatus_OK ) {
		status =  FileStatus_FILE_CANT_BE_CREATED;
	} else {
		outFile << "Windowed= ";
		outFile << m_settings.windowed			<< "\n";
		outFile << "HDRLights= ";
		outFile << m_settings.useHdr			<< "\n";
		outFile << "ScreenHeight= ";
		outFile << m_settings.screenHeight		<< "\n";
		outFile << "ScreenWidth= ";
		outFile << m_settings.screenWidth		<< "\n";
		outFile << "MasterVolume= ";
		outFile << m_settings.masterVolume		<< "\n";
		outFile << "SoundEffectVolume= ";
		outFile << m_settings.sfxVolume			<< "\n";
		outFile << "MusicVolume= ";
		outFile << m_settings.musicVolume		<< "\n";
		outFile << "Rumble= ";
		outFile << m_settings.rumble			<< "\n";
		outFile << "PlayerName= ";
		outFile << m_settings.playerName		<< "\n";
		outFile << "LastUsedIP= ";
		outFile << m_settings.ip				<< "\n";
		outFile << "LastUsedPort= ";
		outFile << m_settings.port				<< "\n";
		outFile << "EnableTesselation= ";
		outFile << m_settings.enableTesselation	<< "\n";
		outFile << "EnableSSAO= ";
		outFile << m_settings.enableSSAO		<< "\n";
		outFile << "EnableCheats= ";
		outFile << m_settings.enableCheats		<< "\n";
		outFile << "EnableVSYNC= ";
		outFile << m_settings.enableVSYNC		<< "\n";

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

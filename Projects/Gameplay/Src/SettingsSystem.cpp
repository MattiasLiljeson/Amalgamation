#include "SettingsSystem.h"

SettingsSystem::SettingsSystem() : EntitySystem( SystemType::SettingsSystem )
{
	m_settingsAreSet = false;
}

SettingsSystem::~SettingsSystem()
{
}

SettingsSystem::FileStatus SettingsSystem::readSettingsFile( string p_filePath /*= "settings.txt" */ )
{
	FileStatus status = FileStatus_OK;

	ifstream inFile;
	inFile.open( p_filePath );

	if( checkFileReadOperation( &inFile ) != FileStatus_OK ) {
		status =  FileStatus_FILE_NOT_FOUND;
	} else {
		inFile >> m_settings.windowed;
		inFile >> m_settings.useHdr;
		inFile >> m_settings.screenHeight;
		inFile >> m_settings.screenWidth;
		inFile >> m_settings.sfxVolume;
		inFile >> m_settings.musicVolume;
		inFile >> m_settings.rumble;
		inFile >> m_settings.playerName;
		inFile >> m_settings.favouriteIp[0];
		inFile >> m_settings.favouriteIp[1];
		inFile >> m_settings.favouriteIp[2];
		inFile >> m_settings.favouriteIp[3];
		status = checkFileReadOperation( &inFile );
		m_settingsAreSet = true;
	}

	inFile.close();

	return status;
}


SettingsSystem::FileStatus SettingsSystem::writeSettingsFile( string p_filePath /*= "settings.txt" */ )
{
	FileStatus status = FileStatus_OK;

	ofstream outFile;
	outFile.open( p_filePath );

	if( checkFileWriteOperation( &outFile ) != FileStatus_OK ) {
		status =  FileStatus_FILE_CANT_BE_CREATED;
	} else {
		outFile << m_settings.windowed << "\n";
		outFile << m_settings.useHdr << "\n";
		outFile << m_settings.screenHeight << "\n";
		outFile << m_settings.screenWidth << "\n";
		outFile << m_settings.sfxVolume << "\n";
		outFile << m_settings.musicVolume << "\n";
		outFile << m_settings.rumble << "\n";
		outFile << m_settings.playerName << "\n";
		outFile << m_settings.favouriteIp[0] << "\n";
		outFile << m_settings.favouriteIp[1] << "\n";
		outFile << m_settings.favouriteIp[2] << "\n";
		outFile << m_settings.favouriteIp[3] << "\n";
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

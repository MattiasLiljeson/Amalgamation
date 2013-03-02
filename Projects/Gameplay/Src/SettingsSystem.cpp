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
		/*
		inFile >> m_settings.favouriteIp[0];
		inFile >> m_settings.favouriteIp[1];
		inFile >> m_settings.favouriteIp[2];
		inFile >> m_settings.favouriteIp[3];
		*/
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
		outFile << m_settings.windowed			<< " ";
		outFile << m_settings.useHdr			<< "\n";
		outFile << m_settings.screenHeight		<< " ";
		outFile << m_settings.screenWidth		<< "\n";
		outFile << m_settings.sfxVolume			<< " ";
		outFile << m_settings.musicVolume		<< " ";
		outFile << m_settings.rumble			<< "\n";
		outFile << m_settings.playerName		<< "\n";
		outFile << m_settings.favouriteIp[0]	<< " ";
		outFile << m_settings.favouriteIp[1]	<< " ";
		outFile << m_settings.favouriteIp[2]	<< " ";
		outFile << m_settings.favouriteIp[3]	<< "\n";
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

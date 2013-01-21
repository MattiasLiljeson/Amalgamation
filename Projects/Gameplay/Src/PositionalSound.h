#pragma once

#include <Component.h>
#include <string>
using namespace std;

// =======================================================================================
//	PositionalSound
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief description...
///        
/// # PositionalSound
/// Detailed description...
/// Created on: 18-1-2013 
///---------------------------------------------------------------------------------------

class PositionalSound: public Component
{
public:
	///-----------------------------------------------------------------------------------
	/// Create a positional sound by providing a file path 
	/// \param p_fileName
	/// \param p_soundVolume
	/// \return 
	///-----------------------------------------------------------------------------------
	PositionalSound(const string p_filePath, const bool p_loop, const float p_soundVolume=1.0f );

	const string& getFileName();
	void setFileName(const string p_filePath);

	const float& getSoundVolume();
	void setSoundVolume(const float p_soundVolume);

	const int& getFileId();
	void setFileId(const int p_fileId);

	const float& getLifeTime();
	void setLifeTime(const float p_lifeTime);
	void decreaseLifeTime(const float& p_delta);

private:
	string m_filePath;
	float m_soundVolume;
	bool m_loop;

	// Controlled by the Audio system(s) itself/themselves.
	int m_fileId;
	float m_lifeTime;

};
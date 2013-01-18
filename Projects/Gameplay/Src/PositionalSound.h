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
	PositionalSound( string p_filePath, bool p_loop, float p_soundVolume=1.0f )
	{
		m_filePath = p_filePath;
		m_loop = p_loop;
		m_soundVolume = p_soundVolume;
	}

	const string& getFileName() {
		return m_filePath;
	}
	void setFileName(string p_filePath) {
		m_filePath = p_filePath;
	}

	const float& getSoundVolume() {
		return m_soundVolume;
	}
	void setSoundVolume(float p_soundVolume) {
		m_soundVolume = p_soundVolume;
	}

	const int& getFileId() {
		return m_fileId;
	}
	void setFileId(int p_fileId) {
		m_fileId = p_fileId;
	}

	const float& getLifeTime() {
		return m_lifeTime;
	}
	void setLifeTime(float p_lifeTime) {
		m_lifeTime = p_lifeTime;
	}
	void decreaseLifeTime(const float& p_delta) {
		m_lifeTime -= p_delta;
	}

private:
	string m_filePath;
	float m_soundVolume;
	bool m_loop;

	// Controlled by the Audio system(s) itself/themselves.
	int m_fileId;
	float m_lifeTime;

};
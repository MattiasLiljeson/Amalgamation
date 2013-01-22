#pragma once

#include <Component.h>
#include <string>
using namespace std;

// =======================================================================================
//	PositionalSoundEffect
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief description...
///        
/// # PositionalSoundEffect
/// Detailed description...
/// Created on: 18-1-2013 
///---------------------------------------------------------------------------------------

class PositionalSoundEffect: public Component
{
public:
	///-----------------------------------------------------------------------------------
	/// Create a positional sound component by providing a file path 
	/// \param p_fileName
	/// \param p_soundVolume
	/// \return 
	///-----------------------------------------------------------------------------------
	PositionalSoundEffect(string p_filePath, bool p_loop = false,
		float p_soundVolume=1.0f );

	const string& getFileName();
	void setFileName(const string p_filePath);

	const float& getSoundVolume();
	void setSoundVolume(const float p_soundVolume);

	const int& getSoundIndex();
	void setSoundIndex(const int p_soundIndex);

	const float& getLifeTime();
	void setLifeTime(const float p_lifeTime);
	void decreaseLifeTime(const float& p_delta);

	const bool& loops() const;

private:
	string m_filePath;
	float m_soundVolume;
	bool m_loop;

	// Controlled by the Audio system(s) itself/themselves.
	int m_soundIndex;
	float m_lifeTime;

};
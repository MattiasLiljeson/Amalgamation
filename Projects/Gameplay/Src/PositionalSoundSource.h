#pragma once

#include <Component.h>
#include <string>
#include <ComponentFactory.h>
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

class PositionalSoundSource: public Component
{
public:
	PositionalSoundSource();
	///-----------------------------------------------------------------------------------
	/// Create a positional sound component by providing a file path 
	/// \param p_fileName
	/// \param p_soundVolume
	/// \return 
	///-----------------------------------------------------------------------------------
	PositionalSoundSource(string p_path, string p_filename, bool p_loop = true,
		float p_soundVolume=1.0f );

	const string& getPath();
	void setPath(const string p_path);
	const string& getFilename();
	void setFilename(const string p_filename);

	const float& getSoundVolume();
	void setSoundVolume(const float p_soundVolume);

	const int& getSoundIndex();
	void setSoundIndex(const int p_soundIndex);

	const float& getLifeTime();
	void setLifeTime(const float p_lifeTime);
	void decreaseLifeTime(const float& p_delta);

	const bool& loops() const;
	void init( vector<ComponentData> p_initData );

private:
	string m_path;
	string m_filename;
	float m_soundVolume;
	bool m_loop;

	// Controlled by the Audio system(s) itself/themselves.
	int m_soundIndex;
	float m_lifeTime;
	
	static ComponentRegister<PositionalSoundSource> s_reg;
};
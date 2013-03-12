#pragma once

#include <Component.h>
#include <AudioHeader.h>
#include <vector>

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

class SoundComponent: public Component
{
public:
	SoundComponent();
	~SoundComponent();
	void addAudioHeader(AudioHeader* p_audioHeader);
	vector<AudioHeader*>* getAllAudiosByType(AudioHeader::SoundType p_soundType);
	AudioHeader* getSoundHeaderByName(AudioHeader::SoundType p_soundType,
		const char* p_name);
	AudioHeader* getSoundHeaderByIndex(AudioHeader::SoundType p_soundType, 
		unsigned int p_index);
	unsigned int getSoundIndexByName(AudioHeader::SoundType p_soundType, const char* p_name);
public:
	vector<vector<AudioHeader*>> m_sounds;
};
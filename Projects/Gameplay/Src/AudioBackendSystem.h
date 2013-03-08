#pragma once
#include <AntTweakBarWrapper.h>
#include "EntitySystem.h"
#include <AudioHeader.h>
#include "SoundOrientation.h"

class SoundWrapper;

using namespace std;

// =======================================================================================
//                                      AudioSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Wraps the sound functionality.
///        
/// # AudioSystem
/// Detailed description.....
/// Created on: 13-12-2012 
///---------------------------------------------------------------------------------------

class AudioBackendSystem : public EntitySystem
{
public:
	AudioBackendSystem();
	virtual ~AudioBackendSystem();

	
	void processEntities(const vector<Entity*>& p_entities);
	void updateListener(const SoundOrientation& p_listenerInfo);

	void changeAudioInstruction(unsigned int p_index, 
		const AudioHeader::PlayState& p_instruction);
	void updateListenerVolume(float p_volume);
	void updateOutputMatrix(int p_index);
	SoundWrapper* getSoundWrapper();
private:
	SoundWrapper* m_soundWrapper;
	string m_label;
};
#pragma once
#include <SoundWrapper.h>
#include "EntitySystem.h"

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
	void initialize();
	void processEntities(const vector<Entity*>& p_entities);
	void updateListener(const SoundSceneInfo& p_listenerInfo);
	int createNonPositionalSound(const char* p_filePath);
	int createPositionalSound(const char* p_filePath, const AglVector3& p_pos );
	void changeAudioInstruction(int p_index, const SoundEnums::Instructions& p_instruction);
private:
	SoundWrapper* m_soundWrapper;
};
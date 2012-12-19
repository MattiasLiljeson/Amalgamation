#pragma once
#include <AntTweakBarWrapper.h>
#include <string>
#include <PositionalSoundCreationInfo.h>
#include <BasicSoundCreationInfo.h>
#include <SoundEnums.h>
#include "EntitySystem.h"

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
	int createAmbientSound(BasicSoundCreationInfo* p_info);
	int createPositionalSound(BasicSoundCreationInfo* p_info,
		PositionalSoundCreationInfo* p_positionalInfo );
	void changeAudioInstruction(int p_index, const SoundEnums::Instructions& p_instruction);
	void updateListenerVolume(float p_volume);
	SoundWrapper* getSoundWrapper();
private:
	SoundWrapper* m_soundWrapper;


	/************************************************************************/
	/* DEBUG FUNCTIONS ONLY! */
	/************************************************************************/
	static AudioBackendSystem* m_theSystem;
private:
	static void TW_CALL stopOrPlaySound(void* p_clientData);
	string m_label;
};
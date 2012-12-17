#pragma once
#include <SoundWrapper.h>
#include <AntTweakBarWrapper.h>
#include <ToString.h>
#include <string>
#include "EntitySystem.h"

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
	void initialize();
	void processEntities(const vector<Entity*>& p_entities);
	void updateListener(const SoundOrientation& p_listenerInfo);
	int createAmbientSound(BasicSoundCreationInfo* p_info);
	int createPositionalSound(BasicSoundCreationInfo* p_info,
		PositionalSoundCreationInfo* p_positionalInfo );
	void changeAudioInstruction(int p_index, const SoundEnums::Instructions& p_instruction);
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
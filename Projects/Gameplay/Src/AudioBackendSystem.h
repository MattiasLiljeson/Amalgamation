#pragma once
#include <AntTweakBarWrapper.h>
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

	///-----------------------------------------------------------------------------------
	/// Play a simple sound effect at a position in the world. The effect cannot be
	/// altered after it has been started.
	/// \param p_position
	/// \return void
	///-----------------------------------------------------------------------------------
	void playPositionalSoundEffect(string p_path, string p_filename,
		AglVector3 p_position);

	///-----------------------------------------------------------------------------------
	/// Play a simple sound effect without position. The effect cannot be altered after
	/// it has been started.
	/// \param p_path
	/// \param p_filename
	/// \return void
	///-----------------------------------------------------------------------------------
	void playSoundEffect(string p_path, string p_filename);

	void processEntities(const vector<Entity*>& p_entities);
	void updateListener(const SoundOrientation& p_listenerInfo);
	int createAmbientSound(BasicSoundCreationInfo* p_info);
	int createPositionalSound(BasicSoundCreationInfo* p_info,
		PositionalSoundCreationInfo* p_positionalInfo );
	void changeAudioInstruction(int p_index, const SoundEnums::Instructions& p_instruction);
	void updateListenerVolume(float p_volume);
	void updateOutputMatrix(int p_index);
	SoundWrapper* getSoundWrapper();
private:
	SoundWrapper* m_soundWrapper;
	string m_label;
	static AudioBackendSystem* m_selfPointer;
private:
	/************************************************************************/
	/* DEBUG FUNCTIONS ONLY! */
	/************************************************************************/
	static void TW_CALL stopOrPlaySound(void* p_clientData);
};
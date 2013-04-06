#pragma once
#include <EntitySystem.h>
#include "AudioBackendSystem.h"

class SoundComponent;
class TimerSystem;
// =======================================================================================
//                                      SoundSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Manages all sounds
///        
/// # SoundSystem
/// Detailed description.....
/// Created on: 4-3-2013 
///---------------------------------------------------------------------------------------

class SoundSystem : public EntitySystem
{
public:
	SoundSystem(AudioBackendSystem* p_audioBackend);
	virtual ~SoundSystem();

	void initialize();

	void processEntities( const vector<Entity*>& p_entities );

	void updateSoundStatus(const SoundComponent* p_soundComponent, bool p_forceUpdate=false);

	void inserted( Entity* p_entity );

	void removed( Entity* p_entity );


private:
	AudioBackendSystem* m_audioBackend;
	TimerSystem* m_timerSystem;
};
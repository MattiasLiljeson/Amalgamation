#pragma once
#include <EntitySystem.h>
#include "AudioBackendSystem.h"

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

	void processEntities( const vector<Entity*>& p_entities );
private:
	AudioBackendSystem* m_audioBackend;
};
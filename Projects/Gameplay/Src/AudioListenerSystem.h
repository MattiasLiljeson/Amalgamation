#pragma once
#include "EntitySystem.h"
#include <SoundOrientation.h>

class AudioBackendSystem;
// =======================================================================================
//                                      AudioListenerSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Handles the updates of listener 
///        
/// # AudioListenerSystem
/// Detailed description.....
/// Created on: 13-12-2012 
///---------------------------------------------------------------------------------------

class AudioListenerSystem : public EntitySystem
{
public:
	AudioListenerSystem(AudioBackendSystem* p_audioBackend);
	virtual ~AudioListenerSystem();
	void processEntities(const vector<Entity*>& p_entities);
	SoundOrientation* getListenerOrientation();

	void initialize();
	void inserted( Entity* p_entity );

private:
	AudioBackendSystem* m_audioBackend;
	SoundOrientation m_listener;
};
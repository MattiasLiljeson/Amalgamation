#pragma once

#include <EntitySystem.h>

class AudioBackendSystem;
class AudioListenerSystem;
class SoundComponent;

// =======================================================================================
//	PositionalSoundSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief description...
///        
/// # PositionalSoundSystem
/// Detailed description...
/// Created on: 18-1-2013 
///---------------------------------------------------------------------------------------

class PositionalSoundSystem: public EntitySystem
{
public:
	PositionalSoundSystem();
	~PositionalSoundSystem();

	void processEntities( const vector<Entity*>& p_entities );
	void initialize();

private:
	void processSoundComponent(Entity* p_entity, SoundComponent* p_soundComponent);
private:
	AudioBackendSystem* m_audioBackendSystem;
	AudioListenerSystem* m_audioListenerSystem;
};
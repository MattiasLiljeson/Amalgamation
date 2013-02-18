#pragma once

#include <EntitySystem.h>
class AudioBackendSystem;

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

	void inserted( Entity* p_entity );
	void removed( Entity* p_entity );

private:
	void updateSoundPositions( Entity* p_entity );

private:
	AudioBackendSystem* m_audioBackendSystem;

};
#pragma once

#include <EntitySystem.h>

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
	void added( Entity* p_entity );
	void deleted( Entity* p_entity );

private:
	AudioBackendSystem* m_audioBackendSystem;

};
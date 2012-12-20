#pragma once
#include "EntitySystem.h"

class AudioBackendSystem;

// =======================================================================================
//                                      AudioController
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Handles the update of sound components
///        
/// # AudioController
/// Detailed description.....
/// Created on: 13-12-2012 
///---------------------------------------------------------------------------------------

class AudioController : public EntitySystem
{
public:
	AudioController(AudioBackendSystem* p_audioBackend);
	virtual ~AudioController();
	void initialize();
	void processEntities(const vector<Entity*>& p_entities);
private:
	AudioBackendSystem* m_audioBackend;
	int m_basicSound;
	int m_basicPositionalSound;
};
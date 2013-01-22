#pragma once

#include <EntitySystem.h>

// =======================================================================================
//                                      ClientPickingSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Creates rays and sends to server
///        
/// # ClientPickingSystem
/// Detailed description.....
/// Created on: 11-12-2012 
///---------------------------------------------------------------------------------------
class ParticleSystemEmitterSystem: public EntitySystem
{
public:
	ParticleSystemEmitterSystem();
	~ParticleSystemEmitterSystem();

	virtual void initialize();
	void processEntities(const vector<Entity*>& p_entities );
};
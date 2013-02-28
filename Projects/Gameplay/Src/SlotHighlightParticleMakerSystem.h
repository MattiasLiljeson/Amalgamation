#pragma once
#include <EntitySystem.h>
#include <AglVector3.h>
class ParticleSystemsComponent;
// =======================================================================================
// SlotHighlightParticleMakerSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief...
///        
/// # SlotHighlightParticleMakerSystem
/// Detailed description...
/// Created on: 28-2-2013 
///---------------------------------------------------------------------------------------
class SlotHighlightParticleMakerSystem: public EntitySystem
{
public:
	SlotHighlightParticleMakerSystem();
	void inserted( Entity* p_entity ) final;

private:
	void createHighlightParticleEmitter( ParticleSystemsComponent* p_emitters,
		AglVector3 p_spawnPosition, AglVector3 p_spawnDirection, int p_desiredIndex );
};
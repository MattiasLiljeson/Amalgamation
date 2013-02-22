#pragma once
#include <EntitySystem.h>
// =======================================================================================
//                       DestroyOnParticlesDeathSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Destroys an entity if it has particles and a DestroyOnParticlesDeath-component
///        
/// # DestroyOnParticlesDeathSystem
/// Detailed description.....
/// Created on: 21-2-2013 
///---------------------------------------------------------------------------------------

class DestroyOnParticlesDeathSystem : public EntitySystem
{
public:
	DestroyOnParticlesDeathSystem();
	virtual ~DestroyOnParticlesDeathSystem();
	virtual void initialize();

	virtual void processEntities( const vector<Entity*>& p_entities );
protected:
private:
};
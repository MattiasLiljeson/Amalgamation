#pragma once
#include <EntitySystem.h>
// =======================================================================================
// OrbitalMovementSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Orbits objects around a center and an axis.
///        
/// # OrbitalMovementSystem
/// Detailed description...
/// Created on: 11-2-2013 
///---------------------------------------------------------------------------------------
class OrbitalMovementSystem: public EntitySystem
{
public:
	OrbitalMovementSystem();
	~OrbitalMovementSystem();
	virtual void processEntities(const vector<Entity*>& p_entities) final;

};
#pragma once
#include <EntitySystem.h>
// =======================================================================================
// GlowAnimationSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Animates the glow of entities having a GlowAnimation component.
///        
/// # GlowAnimationSystem
/// Detailed description...
/// Created on: 11-3-2013 
///---------------------------------------------------------------------------------------
class GlowAnimationSystem: public EntitySystem
{
public:
	GlowAnimationSystem();
	void processEntities( const vector<Entity*>& p_entities ) final;
};
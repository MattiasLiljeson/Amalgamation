#pragma once

#include <Component.h>

// =======================================================================================
//	ShieldModule
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Describes a module that boosts the ship speed
///        
/// # PlayerScore
/// Detailed description...
/// Created on: 4-1-2013 
///---------------------------------------------------------------------------------------

class ShieldModule: public Component
{
public:
	ShieldModule();

public:
	float cooldown;
	float cooldownTime;
	float shieldAge;

};
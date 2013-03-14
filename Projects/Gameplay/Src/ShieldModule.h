#pragma once
#include <Component.h>
#include <ComponentFactory.h>
// =======================================================================================
//	ShieldModule
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief A module that when activated pushes other entities away with great force.
///        
/// # PlayerScore
/// Detailed description...
/// Created on: 4-1-2013 
///---------------------------------------------------------------------------------------

class ShieldModule: public Component
{
public:
	ShieldModule();
	void init( vector<ComponentData> p_initData ) final;

public:
	float cooldown;
	float cooldownTime;
	float activation;
	float activationTime;

	float maxRange;
	float minRange;
	float impulse;

	bool toggleActive;
	bool toggleActiveChange;

public:
	static ComponentRegister<ShieldModule> s_reg;
};
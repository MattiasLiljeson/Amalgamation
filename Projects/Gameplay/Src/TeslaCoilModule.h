#pragma once
#include <Component.h>
// =======================================================================================
// TeslaCoilModule
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief...
///        
/// # TeslaCoilModule
/// Detailed description...
/// Created on: 1-3-2013 
///---------------------------------------------------------------------------------------
class TeslaCoilModule: public Component
{
public:
	TeslaCoilModule();
	TeslaCoilModule(float p_range, float p_optimalRange, float p_cooldownTime,
		float p_damage);

public:
	float range;
	float optimalRange;
	float cooldown;
	float cooldownTime;
	float damage;

};
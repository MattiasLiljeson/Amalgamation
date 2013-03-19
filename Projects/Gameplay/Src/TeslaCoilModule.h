#pragma once
#include <Component.h>
#include <ComponentFactory.h>
// =======================================================================================
// TeslaCoilModule
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief The Tesla coil module has a chance (1%-100%) of electrocuting another module
/// within its range. It has a 100% hit chance within its "optimal" range.
///        
/// # TeslaCoilModule
/// Created on: 1-3-2013 
///---------------------------------------------------------------------------------------
class TeslaCoilModule: public Component
{
public:
	TeslaCoilModule();
	TeslaCoilModule(float p_range, float p_optimalRange, float p_cooldownTime,
		float p_damage);
	void init( vector<ComponentData> p_initData );

public:
	float range;
	float optimalRange;
	float cooldown;
	float cooldownTime;
	float damage;
	bool active;

public:
	static ComponentRegister<TeslaCoilModule> s_reg;
};
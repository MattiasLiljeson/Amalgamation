#pragma once
#include <Component.h>
#include <ComponentFactory.h>
// =======================================================================================
//	Vibration
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Component that vibrates the controller
///        
/// # Vibration
/// Detailed description...
/// Created on: 28-1-2013 
///---------------------------------------------------------------------------------------

class Vibration: public Component
{
public:
	Vibration();
	Vibration(float p_power, float p_minRadius, float p_maxRadius);
	~Vibration(){}
	virtual void init( vector<ComponentData> p_initData );
public:
	float leftPower;
	float rightPower;
	float maxRadius;
	float minRadius;
	float linearDeclineEffectMultiplier; ///< multiplier that will linearly decrease until triggering shutdown of vibration
	bool enabled;
	bool enableDeclineEffect;
	bool distanceAttenuated;
private:
	static ComponentRegister<Vibration> s_reg;
};
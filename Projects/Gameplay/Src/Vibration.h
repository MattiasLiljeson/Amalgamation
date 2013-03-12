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
	float leftPower;	///< Power applied to left motor 0-100
	float rightPower;	///< Power applied to right motor 0-100
	float maxRadius;	///< Maximum radius for 3d vibration
	float minRadius;	///< Minimum radius for 3d vibration
	float linearDeclineEffectMultiplier; ///< multiplier that will linearly decrease until triggering shutdown of vibration
	bool enabled;		///< For switching effect on/off
	bool enableDeclineEffect;	///< Switch on in order to activate linear decline over time
	bool distanceAttenuated;	///< 3d vibration on/off
private:
	static ComponentRegister<Vibration> s_reg;
};
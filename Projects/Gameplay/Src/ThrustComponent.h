#pragma once
#include <Component.h>
#include <AglVector3.h>
// =======================================================================================
//                                      ThrustComponent
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Handles the thrust applied to the object
///        
/// # ThrustComponent
/// Detailed description.....
/// Created on: 7-3-2013 
///---------------------------------------------------------------------------------------

class ThrustComponent : public Component
{
public:
	ThrustComponent();
	~ThrustComponent();

	void addThrustVector(const AglVector3& p_thrust, float p_delta);
	void addAngularVector(const AglVector3& p_angular, float p_delta);
public:
	AglVector3 m_resultingThrust;
	AglVector3 m_resultingAngular;
	float m_thrustPower;
	float m_angularPower;

	const static int POWERCAP = 100;
};
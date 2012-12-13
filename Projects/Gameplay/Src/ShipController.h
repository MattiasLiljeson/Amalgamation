#pragma once

// =======================================================================================
//                                      ShipController
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Ship controller component. Defines controller properties of the ship.
///        
/// # ShipController
/// Detailed description.....
/// Created on: 13-12-2012 
///---------------------------------------------------------------------------------------

#include <Component.h>

class ShipController : public Component
{
public:
	ShipController()
	{
		m_turnSpeed = 1.0f;
		m_thrustPower = 1.0f;
	}

	ShipController(float p_turnSpeed, float p_thrustPower)
	{
		m_turnSpeed = p_turnSpeed;
		m_thrustPower = p_thrustPower;
	}

	~ShipController() {}

	void setTurningSpeed(float p_turnSpeed) {m_turnSpeed = p_turnSpeed;}
	void setThrustPower(float p_thrustPower) {m_thrustPower = p_thrustPower;}

	float getTurningSpeed() {return m_turnSpeed;}
	float getThrustPower() {return m_thrustPower;}

private:
	float m_turnSpeed;
	float m_thrustPower;
};
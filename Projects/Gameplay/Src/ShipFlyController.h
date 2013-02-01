#pragma once

// =======================================================================================
//                                    ShipFlyController
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Ship fly controller component. Defines flying controller 
/// properties of the ship.
///        
/// # ShipFlyController
/// Detailed description.....
/// Created on: 13-12-2012 
///---------------------------------------------------------------------------------------

#include <Component.h>
#include <DebugUtil.h>
#include <ToString.h>
#include <AglVector3.h>

#include <queue>
using namespace std;

class ShipFlyController : public Component
{
public:
	struct TransformGoal
	{
		AglVector3 translation;
		AglQuaternion rotation;
		AglVector3 scale;
		float timestamp;
	};


	ShipFlyController()
	{
		m_turnSpeed = 10.0f;
		m_thrustPower = 0.2f;
		m_currentTransformTimestamp=-1.0f;
	}

	ShipFlyController(float p_turnSpeed, float p_thrustPower)
	{
		m_turnSpeed = p_turnSpeed;
		m_thrustPower = p_thrustPower;
		m_currentTransformTimestamp=-1.0f;
	}

	~ShipFlyController() {}

	void setTurningSpeed(float p_turnSpeed) {m_turnSpeed = p_turnSpeed;}
	void setThrustPower(float p_thrustPower) {m_thrustPower = p_thrustPower;}

	float getTurningSpeed() {return m_turnSpeed;}
	float getThrustPower() {return m_thrustPower;}

	float m_turnSpeed;
	float m_thrustPower;

	AglVector3 m_turnPowerAccumulator;
	AglVector3 m_thrustPowerAccumulator;

	float m_currentTransformTimestamp;
	float m_startTransformTimestamp;
	float m_goalTransformTimestamp;
	AglVector3 m_currentTranslation;
	AglVector3 m_startTranslation;
	AglVector3 m_goalTranslation;
	AglQuaternion m_currentRotation;
	AglQuaternion m_startRotation;
	AglQuaternion m_goalRotation;
	AglVector3 m_currentScale;
	AglVector3 m_startScale;
	AglVector3 m_goalScale;

	queue<TransformGoal> m_transformBuffer;
};
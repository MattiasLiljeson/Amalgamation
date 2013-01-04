#pragma once

#include <EntitySystem.h>

class Control;
class TcpClient;
class InputBackendSystem;
class PhysicsSystem;

// =======================================================================================
//                                ShipControllerSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	System for controlling a ship
///        
/// # ShipControllerSystem
/// Detailed description.....
/// Created on: 13-12-2012 
///---------------------------------------------------------------------------------------

class ShipControllerSystem : public EntitySystem
{
public:
	ShipControllerSystem(InputBackendSystem* p_inputBackend,
						 PhysicsSystem* p_physicsSystem,
						 TcpClient* p_client );
	~ShipControllerSystem();

	virtual void initialize();
	virtual void processEntities( const vector<Entity*>& p_entities );

private:
	float* getControllerEpsilonPointer();

private:
	InputBackendSystem* m_inputBackend;
	PhysicsSystem* m_physics;
	TcpClient* m_client;

	Control* m_horizontalPositive;
	Control* m_horizontalNegative;
	Control* m_verticalPositive;
	Control* m_verticalNegative;
	//
	Control* m_rollRight;
	Control* m_rollLeft;
	Control* m_thrust;
	Control* m_strafeHorizontalPositive;
	Control* m_strafeHorizontalNegative;
	Control* m_strafeVerticalPositive;
	Control* m_strafeVerticalNegative;

	
	// The values of analogue sticks as a vector3 (used in anttweakbar).
	double m_leftStickDir[3];
	double m_rightStickDir[3];
	double m_leftStickDirWithCorrection[3];
	double m_rightStickDirWithCorrection[3];
	
	// Threshold value for the Gamepad's analogue stick error.
	float m_controllerEpsilon;

	// Correction vectors for the left and right thumb sticks.
	double m_leftStickCorrection[2];
	double m_rightStickCorrection[2];
};
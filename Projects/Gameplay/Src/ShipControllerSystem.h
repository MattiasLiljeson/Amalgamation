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
	void initGamePad();
	void initKeyboard();
	void initMouse();

private:
	InputBackendSystem* m_inputBackend;
	PhysicsSystem* m_physics;
	TcpClient* m_client;

	/************************************************************************/
	/* Different Control listeners											*/
	/************************************************************************/

	Control* m_gamepadHorizontalPositive;
	Control* m_gamepadHorizontalNegative;
	Control* m_gamepadVerticalPositive;
	Control* m_gamepadVerticalNegative;

	Control* m_mouseHorizontalPositive;
	Control* m_mouseHorizontalNegative;
	Control* m_mouseVerticalPositive;
	Control* m_mouseVerticalNegative;

	Control* m_gamepadRollRight;
	Control* m_gamepadRollLeft;
	Control* m_gamepadThrust;
	Control* m_gamepadStrafeHorizontalPositive;
	Control* m_gamepadStrafeHorizontalNegative;
	Control* m_gamepadStrafeVerticalNegative;
	Control* m_gamepadStrafeVerticalPositive;

	Control* m_keyboardRollRight;
	Control* m_keyboardRollLeft;
	Control* m_keyboardThrust;
	Control* m_keyboarStrafeHorizontalPos;
	Control* m_keyboarStrafeHorizontalNeg;
	Control* m_keyboardStrafeVerticalPos;
	Control* m_keyboardStrafeVerticalNeg;

	// The values of analogue sticks as a vector3 (used in anttweakbar).
	double m_leftStickDir[3];
	double m_rightStickDir[3];
	double m_leftStickDirWithCorrection[3];
	double m_rightStickDirWithCorrection[3];
	
	// Threshold value for the Gamepad's analogue stick error.
	float m_controllerEpsilon;

	float m_mouseSensitivity;

	// Correction vectors for the left and right thumb sticks.
	double m_leftStickCorrection[2];
	double m_rightStickCorrection[2];
};
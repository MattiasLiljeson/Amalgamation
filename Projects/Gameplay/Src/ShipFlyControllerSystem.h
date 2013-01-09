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

class ShipFlyControllerSystem : public EntitySystem
{
public:
	ShipFlyControllerSystem(InputBackendSystem* p_inputBackend,
						 PhysicsSystem* p_physicsSystem,
						 TcpClient* p_client );
	~ShipFlyControllerSystem();

	virtual void initialize();
	virtual void processEntities( const vector<Entity*>& p_entities );
private:
	struct RawInputForces;		
	struct ResultingInputForces;
private:
	float* getControllerEpsilonPointer();
	void initGamePad();
	void initKeyboard();
	void initMouse();
	void readAllTheInput(RawInputForces& p_outInput);
	void updateAntTweakBar(const ResultingInputForces& p_input);
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

	Control* m_gamepadEditModeTrig;

	Control* m_keyboardRollRight;
	Control* m_keyboardRollLeft;
	Control* m_keyboardThrust;
	Control* m_keyboarStrafeHorizontalPos;
	Control* m_keyboarStrafeHorizontalNeg;
	Control* m_keyboardStrafeVerticalPos;
	Control* m_keyboardStrafeVerticalNeg;

	Control* m_keyboardEditModeTrig;

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
private:
	struct RawInputForces
	{
		double	hPositive,hNegative,
			vPositive,vNegative,
			shPositive,shNegative,
			svPositive,svNegative,
			rRight, rLeft,
			thrust, editSwitchTrig;

		float getHorizontalInput()
		{
			return float(hPositive - hNegative);
		}
		float getVerticalInput()
		{
			return (float)(vPositive - vNegative);
		}
		float getRollInput()
		{
			return (float)(rLeft - rRight);
		}
		float getThrust()
		{
			return (float)(thrust);
		}
		float getStrafeHorizontalInput()
		{
			return (float)(shPositive - shNegative);
		}
		float getStrafeVerticalInput()
		{
			return (float)(svPositive - svNegative);
		}
		float getEditModeSwitch()
		{
			return (float)(editSwitchTrig);
		}
	};

	struct ResultingInputForces
	{
		// Store raw float data
		float horizontalInput,
			verticalInput, 
			rollInput,
			thrustInput,
			strafeHorizontalInput,
			strafeVerticalInput,
			editModeSwitchInput;

		ResultingInputForces(RawInputForces p_rawInput)
		{
			horizontalInput = p_rawInput.getHorizontalInput();
			verticalInput	= p_rawInput.getVerticalInput();
			rollInput	= p_rawInput.getRollInput();
			thrustInput = p_rawInput.getThrust();
			strafeHorizontalInput	= p_rawInput.getStrafeHorizontalInput();
			strafeVerticalInput		= p_rawInput.getStrafeVerticalInput();
			editModeSwitchInput = p_rawInput.getEditModeSwitch();
		}
	};
};
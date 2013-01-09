#pragma once

class Control;
class InputBackendSystem;

// =======================================================================================
//                              ShipSystemsInputHelper
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Input help structs and methods for the ship, collected in one place
///        
/// # ShipSystemsInputHelper
/// Detailed description.....
/// Created on: 9-1-2013 
///---------------------------------------------------------------------------------------

class ShipSystemsInputHelper
{
public:
	ShipSystemsInputHelper();
	virtual ~ShipSystemsInputHelper() {}

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
			stateSwitchInput;

		ResultingInputForces(RawInputForces p_rawInput)
		{
			horizontalInput = p_rawInput.getHorizontalInput();
			verticalInput	= p_rawInput.getVerticalInput();
			rollInput	= p_rawInput.getRollInput();
			thrustInput = p_rawInput.getThrust();
			strafeHorizontalInput	= p_rawInput.getStrafeHorizontalInput();
			strafeVerticalInput		= p_rawInput.getStrafeVerticalInput();
			stateSwitchInput = p_rawInput.getEditModeSwitch();
		}
	};

	void initGamePad(InputBackendSystem* p_inputBackend);
	void initMouse(InputBackendSystem* p_inputBackend);
	void initKeyboard(InputBackendSystem* p_inputBackend);
	float* getControllerEpsilonPointer();
	void readAllTheInput(RawInputForces& p_outInput);
	void updateAntTweakBar(const ResultingInputForces& p_input,
						   InputBackendSystem* p_inputBackend);

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
};
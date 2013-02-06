#pragma once

#include <EntitySystem.h>
#include "ShipInputProcessingSystem.h"

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

class ShipInputProcessingSystem : public EntitySystem
{
public:
	ShipInputProcessingSystem(InputBackendSystem* p_inputBackend);
	virtual ~ShipInputProcessingSystem() {}

	virtual void initialize();
	virtual void process();

	struct RawInputForces
	{
		double	hPositive,hNegative,
			vPositive,vNegative,
			shPositive,shNegative,
			svPositive,svNegative,
			rRight, rLeft,
			thrust;
		bool stateSwitchTrig;

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
		bool getStateModeSwitch()
		{
			return stateSwitchTrig;
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
			strafeVerticalInput;
		bool stateSwitchInput;

		ResultingInputForces() {}

		ResultingInputForces(RawInputForces p_rawInput)
		{
			horizontalInput = p_rawInput.getHorizontalInput();
			verticalInput	= p_rawInput.getVerticalInput();
			rollInput	= p_rawInput.getRollInput();
			thrustInput = p_rawInput.getThrust();
			strafeHorizontalInput	= p_rawInput.getStrafeHorizontalInput();
			strafeVerticalInput		= p_rawInput.getStrafeVerticalInput();
			stateSwitchInput = p_rawInput.getStateModeSwitch();
		}
	};

	ResultingInputForces& getProcessedInput();

private:

	float* getControllerEpsilonPointer();
	RawInputForces readAllInput();

	ResultingInputForces m_processedInput;

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

	// Threshold value for the Gamepad's analogue stick error.
	float m_controllerEpsilon;

	float m_angleInputMultiplier;

	void initGamePad();
	void initMouse();
	void initKeyboard();

	bool m_editSwitchTrigReleased;
	InputBackendSystem* m_inputBackend;
};